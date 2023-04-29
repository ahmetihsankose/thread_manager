# Modular Threads Manager

Modular Threads Manager is a C++ application designed to manage threads and modules in multi-threaded programs using POSIX threads. This project allows you to easily create, manage, and update threads while maintaining modularity.

## Features

* Easy thread creation and management
* Thread priority, affinity, and period update support
* Real-time thread support
* Modular design to facilitate the addition or removal of modules
* Statistics collection and reporting for each thread
* Thread-safe implementation

## Example Usage

```cpp
#include "ThreadManager.h"
#include "WorkerThread.h"
#include "TestModule.h"
#include "ModuleManager.h"
#include <memory>
#include <thread>

int main()
{
    ThreadManager threadManager;
    ModuleManager moduleManager;

    // Create and initialize the TestModule
    std::shared_ptr<TestModule> testModule = moduleManager.createModule<TestModule>();
    moduleManager.initializeModules();

    // Create a WorkerThread with specific parameters and assign the TestModule to it
    // Arguments for createThread:
    //   - threadID (int): Unique identifier for the thread (0 in this case)
    //   - priority (int): Thread priority (0-99)
    //   - period (uint64_t): Thread execution period in nanoseconds (1e9 or 1 second in this case)
    //   - affinity (int): Thread affinity mask (0 for no affinity)
    //   - obj (std::shared_ptr<void>): Shared pointer to the module object (testModule in this case)
    //   - lambda function: A function that calls the 'tick' method of the TestModule
    threadManager.createThread<WorkerThread>(0, 45, 1e9, 0, testModule, [](std::shared_ptr<void> obj)
                                             { std::static_pointer_cast<TestModule>(obj)->tick(); });

    threadManager.startAllThreads();
    threadManager.setRecordStats(0, true);
    threadManager.writeAllThreadStatsToFile("test.txt");


    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Clean up before exiting
    threadManager.stopAllThreads();
    moduleManager.terminateModules();
}
```

## Installation and Dependencies

To use Modular Threads Manager in your project, clone the repository and include the necessary header and source files in your project. The project is designed to work with POSIX threads and should work with any C++ compiler that supports POSIX threads.

</pre>
