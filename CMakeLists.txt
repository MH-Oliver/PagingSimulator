cmake_minimum_required(VERSION 3.28)
project(PagingSimulator)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(PagingSimulator
        main.cpp
        src/des/Event.cpp
        src/des/EventQueue.cpp
        src/core/PagingAlgorithm.h
        src/core/CoreStructs.h
        src/core/MemoryAccessEvent.cpp
        src/core/MemoryAccessEvent.h
        src/Simulation.cpp
        src/Simulation.h
        test/HandleMemoryAccessTest.h
        test/DesTest.h
        src/core/DummyPagingAlgorithm.h
        src/core/Algorithms/FIFOAlgorithm.cpp
        src/core/Algorithms/FIFOAlgorithm.h
        src/core/Algorithms/SecondChanceAlgorithm.cpp
        src/core/Algorithms/SecondChanceAlgorithm.h
        src/core/Algorithms/LRUAlgorithm.cpp
        src/core/Algorithms/LRUAlgorithm.h
        src/core/Algorithms/NRUAlgorithm.cpp
        src/core/Algorithms/NRUAlgorithm.h
        src/core/Algorithms/NFUAlgorithm.cpp
        src/core/Algorithms/NFUAlgorithm.h)
