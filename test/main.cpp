#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "ThreadManager.h"
#include "WorkerThread.h"
#include "TestModule.h"
#include "TestModule1.h"
#include "TestModule2.h"
#include "Logger.h"
#include "Exception.h"

ThreadManager threadManager;
std::shared_ptr<TestModule> testModule = std::make_shared<TestModule>();
std::shared_ptr<TestModule1> testModule1 = std::make_shared<TestModule1>();
std::shared_ptr<TestModule2> testModule2 = std::make_shared<TestModule2>();

int main()
{
    try
    {
        LOG_FILE("/home/kose/workspace/modular-threads-manager/logfile.txt");

        threadManager.createThread<WorkerThread>(0, 45, 1e9, 0, testModule, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule>(obj)->tick(); });

        threadManager.createThread<WorkerThread>(1, 45, 1e9, 0, testModule1, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule>(obj)->tick(); });

        threadManager.createThread<WorkerThread>(2, 45, 1e9, 0, testModule2, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule>(obj)->tick(); });

        threadManager.startAllThreads();
        threadManager.setRecordStats(0, true);
        threadManager.setRecordStats(1, true);
        threadManager.setRecordStats(2, true);
        threadManager.writeAllThreadStatsToFile("/home/kose/workspace/modular-threads-manager/test.txt");
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        
    }
    catch (const Exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
    }
}