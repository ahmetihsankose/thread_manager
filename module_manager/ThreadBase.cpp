/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadBase.h"

void ThreadBase::start()
{
    if (mThreadStarted)
    {
        throw std::runtime_error("Thread is already started");
    }

    mThreadStarted = true;
    if (pthread_create(&mThread, nullptr, &ThreadBase::threadEntryPoint, this) != 0)
    {
        mThreadStarted = false;
        throw std::runtime_error("Failed to start the thread");
    }
}

void ThreadBase::stop()
{
    if (!mThreadStarted)
    {
        throw std::runtime_error("Thread is not started");
    }
    pthread_cancel(mThread);
    pthread_join(mThread, nullptr);
    mThreadStarted = false;
}
