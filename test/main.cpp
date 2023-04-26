#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "ThreadManager.h"
#include "WorkerThread.h"
#include "TestModule.h"

ThreadManager threadManager;
std::shared_ptr<TestModule> testModule = std::make_shared<TestModule>();

int main()
{
    threadManager.createThread<WorkerThread>(0, 45, 1e9, 0, testModule, [](std::shared_ptr<void> obj)
                                             { std::static_pointer_cast<TestModule>(obj)->tick(); });

    // threadManager.createThread<WorkerThread>(2, 45, 1e9, 0, testModule, [](std::shared_ptr<void> obj)
    //                                          { std::static_pointer_cast<TestModule>(obj)->tick(); });

    threadManager.startAllThreads();
    threadManager.setRecordStats(0, true);

    while (true)
    {
        threadManager.printAllThreadStats();
        threadManager.writeAllThreadStatsToFile("/home/kose/workspace/modul-based-thread-manager/test.txt");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        // threadManager.printAllThreadStats();
    }
}