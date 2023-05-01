cmake_minimum_required(VERSION 3.22)

project(thread_manager
    DESCRIPTION "Thread Manager POSIX implementation"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UTILITIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../utilities)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        UTILITIES_DIR
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ThreadManager.cpp
        ThreadBase.cpp
        RealTimeThread.cpp
        StatsCollector.cpp
)