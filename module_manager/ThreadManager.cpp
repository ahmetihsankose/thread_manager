/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadManager.h"

void ThreadManager::startAllThreads()
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto &thread : mThreads)
    {
        thread->start();
    }
}

void ThreadManager::stopAllThreads()
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto &thread : mThreads)
    {
        thread->stop();
    }
}

bool ThreadManager::areAllThreadsRunning()
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (const auto &thread : mThreads)
    {
        if (!thread->isRunning())
        {
            return false;
        }
    }
    return true;
}

void ThreadManager::destroyThread(int threadID)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mThreads.erase(std::remove_if(mThreads.begin(), mThreads.end(),
                                  [threadID](const std::unique_ptr<ThreadBase> &thread)
                                  {
                                      return thread->getThreadID() == threadID;
                                  }),
                   mThreads.end());
}

void ThreadManager::startThread(int threadID)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->start();
    }
}

void ThreadManager::stopThread(int threadID)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->stop();
    }
}

void ThreadManager::updateThreadPriority(int threadID, int newPriority)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->setThreadPriority(newPriority);
    }
}

void ThreadManager::updateThreadPeriod(int threadID, int newPeriod)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->setPeriod(newPeriod);
    }
}

void ThreadManager::updateThreadAffinity(int threadID, int newCPUCore)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->setThreadAffinity(newCPUCore);
    }
}

void ThreadManager::setRecordStats(int threadID, bool recordStats)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->setRecordStats(recordStats);
    }
}

void ThreadManager::printAllThreadStats(bool printToConsole)
{
    std::unique_lock<std::mutex> lock(mMutex);

    for (const auto &thread : mThreads)
    {
        thread->getStatsCollector()->setEnablePrinting(printToConsole);
    }
}

void ThreadManager::writeAllThreadStatsToFile(const std::string &filename)
{
    std::unique_lock<std::mutex> lock(mMutex);

    for (const auto &thread : mThreads)
    {
        thread->getStatsCollector()->setEnableWritingToFile(true, filename);
    }
}
