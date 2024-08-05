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
        statsCollector = recordStats ? std::make_unique<StatsCollector>() : std::move(statsCollector);
    }

    bool isRecordingStats() const override { return mRecordStats.load(); }
    uint64_t getCurrentExecutionTime() const override { return executionTimeNs; }

protected:
    void run() override
    {
        setThreadPriority(mThreadPriority);
        setThreadAffinity(mThreadCPUCore);

        while (isRunning())
        {
            uint64_t startTimeNs = getCurrentTimeNs();
            mWork(mObj);
            uint64_t endTimeNs = getCurrentTimeNs();

            executionTimeNs = endTimeNs - startTimeNs;

            if (mRecordStats.load())
            {
                statsCollector->recordStats(mThreadID, executionTimeNs);
            }
            waitForNextPeriod();
        }
    }

private:
    uint64_t executionTimeNs = 0;
    std::shared_ptr<void> mObj;
    std::function<void(std::shared_ptr<void>)> mWork;
    std::atomic<bool> mRecordStats;
};
