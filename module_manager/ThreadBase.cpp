/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadBase.h"

void ThreadBase::start()
{
    if (mThreadStarted)
    {
        LOG_ERROR("Thread is already started");
        throw std::runtime_error("Thread is already started");
    }

    mThreadStarted = true;
    if (pthread_create(&mThread, nullptr, &ThreadBase::threadEntryPoint, this) != 0)
    {
        mThreadStarted = false;
        LOG_ERROR("Failed to start the thread");
        throw std::runtime_error("Failed to start the thread");
    }
    else
    {
        LOG_INFO("Thread successfully started");
    }
}

void ThreadBase::stop()
{
    if (!mThreadStarted)
    {
        LOG_ERROR("Thread is not started");
        throw std::runtime_error("Thread is not started");
    }
    pthread_cancel(mThread);
    pthread_join(mThread, nullptr);
    mThreadStarted = false;
    LOG_INFO("Thread successfully stopped");
}
