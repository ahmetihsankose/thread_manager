/*
    Written by Ahmet Ihsan KOSE, Istanbul, Turkey
    Contact koseahmetihsan@gmail.com
*/

#pragma once
#include "ThreadBase.h"
#include <pthread.h>
#include <time.h>
#include "../utilities/Logger.h"

class RealTimeThread : public ThreadBase
{
public:
    RealTimeThread() = default;
    ~RealTimeThread() = default;

    void setPeriod(unsigned int periodNs) override;

    bool setThreadPriority(int priority) override;
    bool setThreadAffinity(int cpuCore) override;
    
    const int getThreadCycleTimeNs() const override { return mPeriodNs; }
protected:
    void waitForNextPeriod();
    
    uint64_t getCurrentTimeNs();
    uint64_t mPeriodNs = 0;

private:

    unsigned int mLastExecutionTimeNs = 0;
};