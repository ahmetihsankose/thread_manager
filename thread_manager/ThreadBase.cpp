/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "ThreadBase.h"

void ThreadBase::create()
{
    if (mThreadCreated)
    {
        LOG_ERROR("Thread %d is already created", mThreadID);
        return;
    }

    if (pthread_create(&mThread, nullptr, &ThreadBase::threadEntryPoint, this) != 0)
    {
        LOG_ERROR("Failed to create the thread %d", mThreadID);
        throw std::runtime_error("Failed to create the thread");
    }

    mThreadCreated = true;
    LOG_INFO("Thread %d successfully created", mThreadID);
}

void ThreadBase::start()
{
    if (!mThreadCreated)
    {
        LOG_DEBUG("Thread %d is not created yet", mThreadID);
        create();
    }

    if (mThreadStarted)
    {
        LOG_ERROR("Thread %d is already started", mThreadID);
        return;
    }

    mThreadStarted = true;
    LOG_INFO("Thread %d successfully started", mThreadID);
}

void ThreadBase::stop()
{
    if (!mThreadStarted)
    {
        LOG_DEBUG("Thread %d is not started", mThreadID);
        return;
    }

    pthread_cancel(mThread);
    pthread_join(mThread, nullptr);
    mThreadStarted = false;
    LOG_INFO("Thread %d successfully stopped", mThreadID);
}

void ThreadBase::destroy()
{
    if (!mThreadCreated)
    {
        LOG_DEBUG("Thread %d is not created", mThreadID);
        return;
    }

    if (mThreadStarted)
    {
        stop();
    }

    pthread_detach(mThread);
    mThreadCreated = false;
    LOG_INFO("Thread %d successfully destroyed", mThreadID);
}
