cmake_minimum_required(VERSION 3.28)
project(PagingSimulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(PagingSimulator
        main.cpp
        des/Event.cpp
        des/EventQueue.cpp
        core/PagingAlgorithm.h
        core/CoreStructs.h
        core/MemoryAccessEvent.cpp
        core/MemoryAccessEvent.h
        Simulation.cpp
        Simulation.h)
