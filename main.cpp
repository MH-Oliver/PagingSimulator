/**
* @file main.cpp
 * @brief CLI demo: quick test and trace-driven run.
 */
#include <memory>
#include <iostream>

#include "des/EventQueue.h"
#include "Simulation.h"
#include "TraceLoader.h"
#include "test/HandleMemoryAccessTest.h"

#include "core/algorithms/FIFOAlgorithm.h"
#include "core/algorithms/SecondChanceAlgorithm.h"
#include "core/algorithms/LRUAlgorithm.h"
#include "core/algorithms/NRUAlgorithm.h"
#include "core/algorithms/NFUAlgorithm.h"
#include "core/algorithms/NFUNoAgingAlgorithm.h"

int main() {
    // 1) Quick test sequence
    {
        auto algo = std::make_unique<FIFOAlgorithm>();
        HandleMemoryAccessTest::executeTests(std::move(algo));
    }

    // 2) Trace-driven run
    const int NUM_FRAMES     = 4;
    const int TLB_CAPACITY   = 2;
    const int NUM_VIRTUAL_PAGES = 32;
    const std::string TRACE_FILE = "resources/trace.txt";

    auto algo = std::make_unique<LRUAlgorithm>();
    Simulation sim(NUM_FRAMES, std::move(algo), TLB_CAPACITY);
    Process p1(1, NUM_VIRTUAL_PAGES);
    sim.setCurrentProcess(&p1);

    EventQueue q;
    loadTrace(TRACE_FILE, q, &sim, 1.0, 1.0);
    q.run();

    sim.printStatistics();
    return 0;
}
