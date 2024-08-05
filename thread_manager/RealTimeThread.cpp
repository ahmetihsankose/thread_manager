/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#include "RealTimeThread.h"
#include <time.h>
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <stdexcept>

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

    if (result != 0)
    {
        LOG_ERROR("Thread %d failed to sleep for next period. Error: %d - %s", getThreadID(), result, strerror(result));
    }
}

uint64_t RealTimeThread::getCurrentTimeNs()
{
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    return static_cast<uint64_t>(currentTime.tv_sec) * 1e9 + static_cast<uint64_t>(currentTime.tv_nsec);
}

bool RealTimeThread::setThreadPriority(int priority)
{
    sched_param param;
    param.sched_priority = priority;
    int errorFlag = pthread_setschedparam(mThread, SCHED_FIFO, &param);
    if (errorFlag != 0)
    {
        LOG_ERROR("Could not set thread %d priority to %d. Error: %d - %s. Check limits.conf or execute as root!", getThreadID(), priority, errorFlag, strerror(errorFlag));
        return false;
    }
    LOG_INFO("Thread %d priority set to %d", getThreadID(), priority);
    return true;
}

bool RealTimeThread::setThreadAffinity(int cpuCore)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int numberOfCPUs = sysconf(_SC_NPROCESSORS_ONLN);

    if (cpuCore >= 0 && cpuCore < numberOfCPUs)
    {
        CPU_SET(cpuCore, &cpuset);
        int errorFlag = pthread_setaffinity_np(mThread, sizeof(cpuset), &cpuset);
        if (errorFlag != 0)
        {
            LOG_ERROR("Could not assign thread %d to cpu core %d. Error: %d - %s", getThreadID(), cpuCore, errorFlag, strerror(errorFlag));
            return false;
        }
        LOG_INFO("Thread %d assigned to cpu core %d", getThreadID(), cpuCore);
        return true;
    }
    LOG_ERROR("Invalid CPU core %d for thread %d. Available cores: %d", cpuCore, getThreadID(), numberOfCPUs);
    return false;
}
