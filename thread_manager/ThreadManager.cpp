/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadManager.h"
#include "../utilities/Logger.h"

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

void ThreadManager::destroyAllThreads()
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto &thread : mThreads)
    {
        thread->destroy();
    }
    mThreads.clear();
}

bool ThreadManager::areAllThreadsRunning()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return std::all_of(mThreads.begin(), mThreads.end(),
                       [](const std::unique_ptr<ThreadBase> &thread)
                       {
                           return thread->isRunning();
                       });
}

bool ThreadManager::areAllThreadsStopped()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return std::all_of(mThreads.begin(), mThreads.end(),
                       [](const std::unique_ptr<ThreadBase> &thread)
                       {
                           return !thread->isRunning();
                       });
}

void ThreadManager::destroyThread(int threadID)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    if (it != mThreads.end())
    {
        (*it)->destroy();
        mThreads.erase(it);
        LOG_INFO("Thread %d successfully destroyed", threadID);
    }
    else
    {
        LOG_ERROR("Thread %d does not exist", threadID);
    }
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
        auto statsCollector = thread->getStatsCollector().get();
        if (statsCollector)
        {
            statsCollector->setEnablePrinting(printToConsole);
        }
    }
}

void ThreadManager::writeAllThreadStatsToFile(const std::string &filename)
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (const auto &thread : mThreads)
    {
        auto statsCollector = thread->getStatsCollector().get();
        if (statsCollector)
        {
            statsCollector->setEnableWritingToFile(true, filename);
        }
    }
}

bool ThreadManager::isThreadExist(int threadID)
{
    std::unique_lock<std::mutex> lock(mMutex);
    auto it = std::find_if(mThreads.begin(), mThreads.end(),
                           [threadID](const std::unique_ptr<ThreadBase> &thread)
                           {
                               return thread->getThreadID() == threadID;
                           });
    return it != mThreads.end();
}
