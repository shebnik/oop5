cmake_minimum_required(VERSION 3.10)
set(THREADS_PREFER_PTHREAD_FLAG ON)

project(main)

find_package(PThreads4W REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE PThreads4W::PThreads4W)