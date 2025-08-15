cmake_minimum_required(VERSION 3.22)
project(PagingSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable reasonable warnings (tighten as needed)
if(MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Core library ---
add_library(PagingCore
        src/des/Event.cpp
        src/des/EventQueue.cpp
        src/core/MemoryAccessEvent.cpp
        src/Simulation.cpp
        src/TraceLoader.cpp
        src/core/algorithms/FIFOAlgorithm.cpp
        src/core/algorithms/LRUAlgorithm.cpp
        src/core/algorithms/NRUAlgorithm.cpp
        src/core/algorithms/NFUAlgorithm.cpp
        src/core/algorithms/NFUNoAgingAlgorithm.cpp
        src/core/algorithms/SecondChanceAlgorithm.cpp
)

target_include_directories(PagingCore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- CLI demo executable ---
add_executable(PagingSimulatorCli
        main.cpp
        test/HandleMemoryAccessTest.h
)
target_link_libraries(PagingSimulatorCli PRIVATE PagingCore)

# Copy sample trace after build
add_custom_command(TARGET PagingSimulatorCli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:PagingSimulatorCli>/resources
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/resources/trace.txt
        $<TARGET_FILE_DIR:PagingSimulatorCli>/resources/trace.txt
)
