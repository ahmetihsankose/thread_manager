#include "StatsCollector.h"

void StatsCollector::recordStats(int id, uint64_t value)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);
    auto &ringBuffer = mStats[id];
    ringBuffer.push_back(value);

    mTotalTaskCount[id]++; // Increase the total task count for the given ID

    printStats();
    writeStatsToFile(mFilename);
    if (ringBuffer.size() == RING_BUFFER_SIZE - 1)
    {
        std::cout << "Ring buffer is full, clearing it..." << std::endl;
        ringBuffer.clear();
    }
}

void StatsCollector::printStats()
{
    if (!enablePrinting)
    {
        return;
    }

    for (const auto &entry : mStats)
    {
        int id = entry.first;
        const auto &values = entry.second;
        size_t totalTasks = mTotalTaskCount[id]; // Get the total task count for the given ID

        uint64_t totalValue = 0;
        for (size_t i = 0; i < values.size(); ++i)
        {
            totalValue += values.operator[](i);
        }

        uint64_t averageValue = (values.size() > 0) ? (totalValue / values.size()) : 0;

        std::cout << "ID: " << id
                  << ", Total tasks: " << mTotalTaskCount[id]
                  << ", Average value: " << averageValue << " ns" << std::endl;
    }
}

void StatsCollector::writeStatsToFile(const std::string &filename)
{
    if (!enableWritingToFile)
    {
        return;
    }

    std::ios_base::openmode mode = std::ios::app;

    if (mFileReset)
    {
        mode = std::ios::trunc;
        mFileReset = false;
    }

    std::ofstream outFile(filename, mode);

    if (!outFile.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    for (const auto &entry : mStats)
    {
        int id = entry.first;
        const auto &values = entry.second;

        uint64_t totalValue = 0;
        size_t valueCount = values.size();

        for (size_t i = 0; i < valueCount; ++i)
        {
            totalValue += values.operator[](i);
        }
        uint64_t averageValue = (valueCount > 0) ? (totalValue / valueCount) : 0;

        outFile << "ID: " << id
                << ", Total tasks: " << mTotalTaskCount[id]
                << ", Average value: " << averageValue << " ns" << std::endl;
    }

    outFile.close();
}

void StatsCollector::setEnablePrinting(bool enable)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);
    enablePrinting = enable;
}

void StatsCollector::setEnableWritingToFile(bool enable, const std::string &filename)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);
    mFilename = filename;
    enableWritingToFile = enable;
}