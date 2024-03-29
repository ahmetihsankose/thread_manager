/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadBase.h"

void ThreadBase::start()
{
    if (mThreadStarted)
    {
        LOG_ERROR("Thread %d is already started", mThreadID);
        // throw std::runtime_error("Thread is already started");
        return;
    }

    mThreadStarted = true;
    if (pthread_create(&mThread, nullptr, &ThreadBase::threadEntryPoint, this) != 0)
    {
        mThreadStarted = false;
        LOG_ERROR("Failed to start the thread %d", mThreadID);
        throw std::runtime_error("Failed to start the thread");
    }
    else
    {
        LOG_INFO("Thread %d successfully started", mThreadID);
    }
}

void ThreadBase::stop()
{
    if (!mThreadStarted)
    {
        LOG_ERROR("Thread %d is not started", mThreadID);
        // throw std::runtime_error("Thread is not started");
        return;
    }
    pthread_cancel(mThread);
    pthread_join(mThread, nullptr);
    mThreadStarted = false;
    LOG_INFO("Thread %d successfully stopped", mThreadID);
}
