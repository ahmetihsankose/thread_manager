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
    ThreadBase() : mThread(), mThreadStarted(false), mThreadID(0), mThreadName(""), mThreadPriority(0), mThreadCPUCore(-1) {}
    virtual ~ThreadBase()
    {
        destroy();
    }

    ThreadBase(const ThreadBase &) = delete;
    ThreadBase &operator=(const ThreadBase &) = delete;

    void create();
    void start();
    void stop();
    void destroy();

    bool isRunning() const { return mThreadStarted; }
    void setThreadID(int id) { mThreadID = id; }
    void setThreadName(const std::string &name) { mThreadName = name; }
    void setPriority(int priority) { mThreadPriority = priority; }
    void setCpuCore(int cpuCore) { mThreadCPUCore = cpuCore; }

    int getThreadCpuCore() const { return mThreadCPUCore; }
    int getThreadPriority() const { return mThreadPriority; }
    int getThreadID() const { return mThreadID; }
    const std::string &getThreadName() const { return mThreadName; }

    virtual bool setThreadPriority(int priority) { return true; }
    virtual bool setThreadAffinity(int cpuCore) { return true; }
    virtual void setPeriod(unsigned int periodNs) {}
    virtual void setRecordStats(bool recordStats) {}

    virtual bool isRecordingStats() const { return false; }
    virtual uint64_t getCurrentExecutionTime() const { return 0; }
    virtual int getThreadCycleTimeNs() const { return 0; }

    std::unique_ptr<StatsCollector> &getStatsCollector() { return statsCollector; }

protected:
    virtual void run() = 0;
    std::unique_ptr<StatsCollector> statsCollector = nullptr;

    pthread_t mThread;

private:
    bool mThreadStarted = false;
    bool mThreadCreated = false;
protected:
    int mThreadID = 0;
    std::string mThreadName = "";
    int mThreadPriority = 0;
    int mThreadCPUCore = -1;

private:
    static void *threadEntryPoint(void *arg)
    {
        auto self = static_cast<ThreadBase *>(arg);
        self->run();
        return nullptr;
    }
};
