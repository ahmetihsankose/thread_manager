cmake_minimum_required(VERSION 3.22)

project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/module_manager
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_BINARY_DIR}/module_manager/libmodule_manager.a

)
