/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <pthread.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include "StatsCollector.h"
#include "../utilities/Logger.h"
#include <memory>

class ThreadBase
{
public:
    ThreadBase() : mThread(), mThreadStarted(false), mThreadID(0), mThreadPriority(0), mThreadCPUCore(-1) {}

    virtual ~ThreadBase()
    {
        if (mThreadStarted)
        {
            pthread_detach(mThread);
        }
    }

    ThreadBase(const ThreadBase &) = delete;
    ThreadBase &operator=(const ThreadBase &) = delete;

    void start();
    void stop();

    bool isRunning() const { return mThreadStarted; }
    void setThreadID(int id) { mThreadID = id; }
    int getThreadCpuCore() const { return mThreadCPUCore; }
    int getThreadPriority() const { return mThreadPriority; }
    int getThreadID() const { return mThreadID; }

    void setPriority(int priority) { mThreadPriority = priority; }
    void setCpuCore(int cpuCore) { mThreadCPUCore = cpuCore; }

    virtual bool setThreadPriority(int priority) { return true; }
    virtual bool setThreadAffinity(int cpuCore) { return true; }
    virtual void setPeriod(unsigned int periodMs) {}
    virtual void setRecordStats(bool recordStats) { return; }

    std::unique_ptr<StatsCollector> &getStatsCollector() { return statsCollector; }


protected:
    virtual void run() = 0;
    std::unique_ptr<StatsCollector> statsCollector = nullptr;

    int mThreadID = 0;
    int mThreadPriority = 0;
    int mThreadCPUCore = -1;
    pthread_t mThread;

private:
    static void *threadEntryPoint(void *arg)
    {
        auto self = static_cast<ThreadBase *>(arg);
        self->run();
        return nullptr;
    }

    bool mThreadStarted = false;
};
