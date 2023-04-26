#include <map>
#include <vector>
#include <mutex>
#include <iostream>
#include <fstream>
#include "RingBuffer.h"

class StatsCollector
{
public:
    void recordStats(int id, uint64_t value);
    void printStats();
    void writeStatsToFile(const std::string &filename);

private:
    std::map<int, RingBuffer<uint64_t, 5>> mStats;
    std::mutex mStatsMutex;
};