cmake_minimum_required(VERSION 3.22)

project(module_manager
    DESCRIPTION "Module Manager POSIX Threads"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/module_manager)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ThreadManager.cpp
        ThreadBase.cpp
        RealTimeThread.cpp
        StatsCollector.cpp
)