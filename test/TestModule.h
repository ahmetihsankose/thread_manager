#pragma once
#include <iostream>
#include <pthread.h>

extern sem_t sem1, sem2, sem3, sem4;

class TestModule1
{
public:
    void tick()
    {
        std::cout << "ticked 1" << std::endl;
        sem_post(&sem1);
        sem_wait(&sem4);
        std::cout << "ticked 5" << std::endl;
    }
};

class TestModule2
{
public:
    void tick()
    {
        sem_wait(&sem1);
        std::cout << "ticked 2" << std::endl;
        sem_post(&sem2);

        sem_wait(&sem3);
        std::cout << "ticked 4" << std::endl;
        sem_post(&sem4);
    }
};

class TestModule3
{
public:
    void tick()
    {
        sem_wait(&sem2);
        std::cout << "ticked 3" << std::endl;
        sem_post(&sem3);
    }
};
