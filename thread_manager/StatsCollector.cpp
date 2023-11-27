/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "StatsCollector.h"

void StatsCollector::recordStats(int id, uint64_t value)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);

    mStats[id].push_back(value);

    mTotalTaskCount[id]++; // Increase the total task count for the given ID

    printStats();
    writeStatsToFile(mFileName);
}

const uint64_t StatsCollector::getCurrentExecutionTime(int id) const
{
    auto it = mStats.find(id);
    if (it != mStats.end() && it->second.size() > 0)
    {
        return it->second.back();
    }
    else
    {
        return 0;
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
    mFileName = filename;
    resetFile(mFileName);
    enableWritingToFile = enable;
}

void StatsCollector::resetFile(const std::string &filename)
{
    std::ofstream outFile(filename, std::ios::trunc);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    outFile.close();
}