/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "RealTimeThread.h"
#include <time.h>
#include <errno.h>

void RealTimeThread::setPeriod(unsigned int periodNs)
{
    if (periodNs <= 0)
    {
        LOG_ERROR("Invalid period value for thread %d", getThreadID());
        throw std::runtime_error("Invalid period value (must be greater than 0)");
    }

    LOG_INFO("Thread %d period set to %d ns", getThreadID(), periodNs);

    mPeriodNs = periodNs;
}

void RealTimeThread::waitForNextPeriod()
{
    struct timespec ts;
    uint64_t sleepTimeNs = mPeriodNs - (getCurrentTimeNs() % mPeriodNs);

    ts.tv_sec = sleepTimeNs / 1000000000;
    ts.tv_nsec = sleepTimeNs % 1000000000;

    int result;
    do
    {
        result = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, &ts);
    } while (result == EINTR);
}

uint64_t RealTimeThread::getCurrentTimeNs()
{
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    return static_cast<uint64_t>(currentTime.tv_sec) * 1e9 + static_cast<uint64_t>(currentTime.tv_nsec);
}

bool RealTimeThread::setThreadPriority(int priority)
{
    bool success = true;
    pthread_t thread = mThread;

    sched_param param;
    param.sched_priority = priority;
    int errorFlag = pthread_setschedparam(thread, SCHED_FIFO, &param);
    if (errorFlag != 0)
    {
        LOG_ERROR("Could not set thread %d priority. Check limits.conf or execute as root!", getThreadID());
        success = false;
    }

    LOG_INFO("Thread %d priority set to %d", getThreadID(), priority);

    return success;
}

bool RealTimeThread::setThreadAffinity(int cpuCore)
{
    bool success = true;
    pthread_t thread = mThread;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int numberOfCPUs = sysconf(_SC_NPROCESSORS_ONLN);

    if (cpuCore >= 0)
    {
        if (cpuCore >= numberOfCPUs)
        {
            LOG_ERROR("Tried to attach thread to core: %d even though we only have: %d core!", cpuCore, numberOfCPUs);
            return false;
        }
        CPU_SET(cpuCore, &cpuset);
        int errorFlag = pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
        if (errorFlag != 0)
        {
            LOG_ERROR("Could not assign thread %d to single cpu core: %d", getThreadID(), errorFlag);
            success = false;
        }
    }

    LOG_INFO("Thread %d assigned to cpu core: %d", getThreadID(), cpuCore);

    return success;
}
