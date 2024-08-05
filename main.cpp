#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "thread_manager/ThreadManager.h"
#include "thread_manager/WorkerThread.h"
#include "TestModule.h"
#include "utilities/Logger.h"
#include "utilities/Exception.h"
#include <semaphore.h>

sem_t sem1, sem2, sem3, sem4;

ThreadManager threadManager;
std::shared_ptr<TestModule1> testModule1 = std::make_shared<TestModule1>();
std::shared_ptr<TestModule2> testModule2 = std::make_shared<TestModule2>();
std::shared_ptr<TestModule3> testModule3 = std::make_shared<TestModule3>();

int main()
{
    try
    {
        // LOG_FILE("/home/kose/workspace/my_projects/thread_manager/logfile.log");

        sem_init(&sem1, 0, 0);
        sem_init(&sem2, 0, 0);
        sem_init(&sem3, 0, 0);
        sem_init(&sem4, 0, 0);

        threadManager.createThread<WorkerThread>(0, "test1", 45, 1e9, 0, testModule1, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule1>(obj)->tick(); });

        threadManager.createThread<WorkerThread>(1, "test2", 45, 1e9, 0, testModule2, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule2>(obj)->tick(); });

        threadManager.createThread<WorkerThread>(2, "test3", 45, 1e9, 0, testModule3, [](std::shared_ptr<void> obj)
                                                 { std::static_pointer_cast<TestModule3>(obj)->tick(); });


        // threadManager.setRecordStats(0, true);
        // threadManager.setRecordStats(1, true);
        // threadManager.setRecordStats(2, true);
        // threadManager.writeAllThreadStatsToFile("/home/kose/workspace/my_projects/thread_manager/thread_test.log");
        threadManager.startAllThreads();

        threadManager.updateThreadPriority(0, 61);




    }
    catch (const Exception &e)
    {
        std::cout << e.what() << std::endl;

    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        threadManager.updateThreadPriority(0, 32);

    }
}