#include "StatsCollector.h"

void StatsCollector::recordStats(int id, uint64_t value)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);
    mStats[id].push_back(value);
}

void StatsCollector::printStats()
{
    std::unique_lock<std::mutex> lock(mStatsMutex);

    for (const auto &entry : mStats)
    {
        int id = entry.first;
        const auto &values = entry.second;

        uint64_t totalValue = 0;
        for (size_t i = 0; i < values.size(); ++i)
        {
            totalValue += values.operator[](i); // TODO - why does this work? and at() doesn't?
        }

        uint64_t averageValue = (values.size() > 0) ? (totalValue / values.size()) : 0;

        std::cout << "ID: " << id
                  << ", Total tasks: " << values.size()
                  << ", Average value: " << averageValue << " ns" << std::endl;
    }
}

void StatsCollector::writeStatsToFile(const std::string &filename)
{
    std::unique_lock<std::mutex> lock(mStatsMutex);
    std::ofstream outFile(filename);

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
            totalValue += values.operator[](i); // TODO - why does this work? and at() doesn't?
        }
        uint64_t averageValue = (valueCount > 0) ? (totalValue / valueCount) : 0;

        outFile << "ID: " << id
                << ", Total tasks: " << valueCount
                << ", Average value: " << averageValue << " ns" << std::endl;
    }

    outFile.close();
}
