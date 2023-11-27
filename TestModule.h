#pragma once
#include <iostream>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

// #define USE_MUTEX_CONDITION_VARIABLE
// #define USE_POSIX_SEMAPHORES
#define USE_PTHREAD_CONDITION_VARIABLE

int step = 1;

extern sem_t sem1, sem2, sem3, sem4;
std::mutex mtx;
std::condition_variable cv;
pthread_mutex_t p_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p_cond = PTHREAD_COND_INITIALIZER;

class TestModule1
{
public:
    void tick()
    {
#ifdef USE_MUTEX_CONDITION_VARIABLE
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return step == 1; });

            std::cout << "ticked 1" << std::endl;
            step = 2;
            lock.unlock();
            cv.notify_all();
        }

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return step == 5; });

            std::cout << "ticked 5" << std::endl;
            step = 1;
            lock.unlock();
            cv.notify_all();
        }
#endif

#ifdef USE_POSIX_SEMAPHORES
        std::cout << "ticked 1" << std::endl;
        sem_post(&sem1);
        sem_wait(&sem4);
        std::cout << "ticked 5" << std::endl;
#endif

#ifdef USE_PTHREAD_CONDITION_VARIABLE
        {
            pthread_mutex_lock(&p_mtx);
            while (step != 1)
            {
                pthread_cond_wait(&p_cond, &p_mtx);
            }

            std::cout << "ticked 1" << std::endl;
            step = 2;
            pthread_mutex_unlock(&p_mtx);
            pthread_cond_broadcast(&p_cond);
        }

        {
            pthread_mutex_lock(&p_mtx);
            while (step != 4)
            {
                pthread_cond_wait(&p_cond, &p_mtx);
            }

            std::cout << "ticked 5" << std::endl;
            step = 1;
            pthread_mutex_unlock(&p_mtx);
            pthread_cond_broadcast(&p_cond);
        }
#endif
    }
};

class TestModule2
{
public:
    void tick()
    {
#ifdef USE_MUTEX_CONDITION_VARIABLE
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return step == 2; });

            std::cout << "ticked 2" << std::endl;
            step = 3;
            lock.unlock();
            cv.notify_all();
        }

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return step == 4; });

            std::cout << "ticked 4" << std::endl;
            step = 5;
            lock.unlock();
            cv.notify_all();
        }
#endif

#ifdef USE_POSIX_SEMAPHORES
        sem_wait(&sem1);
        std::cout << "ticked 2" << std::endl;
        sem_post(&sem2);

        sem_wait(&sem3);
        std::cout << "ticked 4" << std::endl;
        sem_post(&sem4);

#endif

#ifdef USE_PTHREAD_CONDITION_VARIABLE
        {
            pthread_mutex_lock(&p_mtx);
            while (step != 2)
            {
                pthread_cond_wait(&p_cond, &p_mtx);
            }

            std::cout << "ticked 2" << std::endl;
            step = 3;
            pthread_mutex_unlock(&p_mtx);
            pthread_cond_broadcast(&p_cond);
        }

        {
            pthread_mutex_lock(&p_mtx);
            while (step != 5)
            {
                pthread_cond_wait(&p_cond, &p_mtx);
            }

            std::cout << "ticked 4" << std::endl;
            step = 4;
            pthread_mutex_unlock(&p_mtx);
            pthread_cond_broadcast(&p_cond);
        }
#endif
    }
};

class TestModule3
{
public:
    void tick()
    {

#ifdef USE_MUTEX_CONDITION_VARIABLE
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return step == 3; });

            std::cout << "ticked 3" << std::endl;
            step = 4;
            lock.unlock();
            cv.notify_all();
        }
#endif

#ifdef USE_POSIX_SEMAPHORES
        sem_wait(&sem2);
        std::cout << "ticked 3" << std::endl;
        sem_post(&sem3);

#endif

#ifdef USE_PTHREAD_CONDITION_VARIABLE
        {
            pthread_mutex_lock(&p_mtx);
            while (step != 3)
            {
                pthread_cond_wait(&p_cond, &p_mtx);
            }

            std::cout << "ticked 3" << std::endl;
            step = 5;
            pthread_mutex_unlock(&p_mtx);
            pthread_cond_broadcast(&p_cond);
        }
#endif
    }
};
