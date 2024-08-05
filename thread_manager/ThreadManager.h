/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include "ThreadBase.h"
#include <algorithm>

class ThreadManager
{
public:
    ThreadManager() = default;

    template <typename T, typename... Args>
    void createThread(size_t threadIndex, std::string name, int priority, uint64_t periodNs, int cpuCore = -1, Args &&...args)
    {
        std::unique_lock<std::mutex> lock(mMutex);

        auto it = std::find_if(mThreads.begin(), mThreads.end(),
                               [threadIndex](const std::unique_ptr<ThreadBase> &thread)
                               {
                                   return thread->getThreadID() == threadIndex;
                               });
        if (it != mThreads.end())
        {
            LOG_DEBUG("Thread %d already exists", threadIndex);
            return;
        }
        
        auto thread = std::make_unique<T>(std::forward<Args>(args)...);
        thread->setThreadID(threadIndex);
        thread->setThreadName(name);
        thread->setPeriod(periodNs);
        thread->setPriority(priority);
        thread->setCpuCore(cpuCore);
        mThreads.emplace_back(std::move(thread));
    }

    void startAllThreads();
    void stopAllThreads();
    void destroyAllThreads();

    bool areAllThreadsRunning();
    bool areAllThreadsStopped();

    void destroyThread(int threadID);
    void startThread(int threadID);
    void stopThread(int threadID);
    void updateThreadPriority(int threadID, int newPriority);
    void updateThreadAffinity(int threadID, int newCPUCore);
    void updateThreadPeriod(int threadID, int newPeriod);

    void setRecordStats(int threadID, bool recordStats);
    void printAllThreadStats(bool printToConsole = true);
    void writeAllThreadStatsToFile(const std::string &filename);

    bool isThreadExist(int threadID);

    const std::vector<std::unique_ptr<ThreadBase>> &getThreads() const { return mThreads; }

private:
    std::vector<std::unique_ptr<ThreadBase>> mThreads;
    std::mutex mMutex;
};
