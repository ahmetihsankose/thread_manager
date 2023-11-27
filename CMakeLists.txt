cmake_minimum_required(VERSION 3.22)

project(test
    DESCRIPTION "ters for thread manager POSIX"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(thread_manager)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        thread_manager
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        thread_manager
)





