cmake_minimum_required(VERSION 3.28)
project(PagingSimulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(PagingSimulator
        main.cpp
        des/Event.cpp
        des/EventQueue.cpp
        pagingAlgorithms/PagingAlgorithm.h)
