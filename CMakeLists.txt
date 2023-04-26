cmake_minimum_required(VERSION 3.22)

project(module_manager
    DESCRIPTION "Module Manager POSIX Threads"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(module_manager)
add_subdirectory(test)





