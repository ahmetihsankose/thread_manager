/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include "RealTimeThread.h"
#include <functional>
#include <memory>
#include <atomic>

class WorkerThread : public RealTimeThread
{
public:
    explicit WorkerThread(std::shared_ptr<void> obj,
                          std::function<void(std::shared_ptr<void>)> work,
                          bool recordStats = false)
        : mObj(obj), mWork(work), mRecordStats(recordStats) {}

    void setRecordStats(bool recordStats) override
    {
        mRecordStats.store(recordStats);
        statsCollector = recordStats ? std::make_unique<StatsCollector>() : move(statsCollector);
    }

protected:
    void run() override
    {
        setThreadPriority(mThreadPriority);
        setThreadAffinity(mThreadCPUCore);

        while (isRunning())
        {
            uint64_t startTimeNs = getCurrentTimeNs();
            mWork(mObj); // call the work function
            uint64_t endTimeNs = getCurrentTimeNs();

            uint64_t executionTimeNs = endTimeNs - startTimeNs;
            if (mRecordStats.load())
            {
                statsCollector->recordStats(mThreadID, executionTimeNs);
            }
            waitForNextPeriod();
        }
    }

private:
    std::shared_ptr<void> mObj;
    std::function<void(std::shared_ptr<void>)> mWork;
    std::atomic<bool> mRecordStats;
};
