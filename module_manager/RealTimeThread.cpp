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
        throw std::runtime_error("Invalid period value (must be greater than 0)");
    }

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
        std::cout << "Could not set thread priority. Check limits.conf or execute as root" << std::endl;
        success = false;
    }

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
            std::cout << "Tried to attach thread to core: " << cpuCore << " even though we only have: " << numberOfCPUs << " core!" << std::endl;
            return false;
        }
        CPU_SET(cpuCore, &cpuset);
        int errorFlag = pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
        if (errorFlag != 0)
        {
            std::cout << "Could not assign thread " << getThreadID() << " to single cpu core: " << errorFlag << std::endl;
            success = false;
        }
    }

    return success;
}
