/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include <map>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>
#include <fstream>
#include "RingBuffer.h"

constexpr size_t RING_BUFFER_SIZE = 1024;

class StatsCollector
{
public:
    void recordStats(int id, uint64_t value);
    void setEnablePrinting(bool enable);
    void setEnableWritingToFile(bool enable, const std::string &filename);
    void resetFile(const std::string &filename);

private:
    std::map<int, RingBuffer<uint64_t, RING_BUFFER_SIZE>> mStats = {};
    std::unordered_map<int, size_t> mTotalTaskCount = {};
    std::mutex mStatsMutex;
    void printStats();
    void writeStatsToFile(const std::string &filename);
    bool enablePrinting = false;
    bool enableWritingToFile = false;
    bool mFileReset = true;
    std::string mFileName;
};
