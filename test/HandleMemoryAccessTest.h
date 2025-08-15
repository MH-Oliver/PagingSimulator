/**
* @file HandleMemoryAccessTest.h
 * @brief Quick, inline test driver for Simulation::handleMemoryAccess.
 */
#ifndef HANDLEMEMORYACCESSTEST_H
#define HANDLEMEMORYACCESSTEST_H

#include <iostream>
#include <memory>
#include "Simulation.h"
#include "core/PagingAlgorithm.h"


/**
 * @brief Minimal sanity-check sequence for the Simulation.
 */
class HandleMemoryAccessTest {
public:
    /**
     * @brief Run a short fixed sequence of accesses.
     * @param algo Replacement algorithm (ownership transferred).
     */
    static void executeTests(std::unique_ptr<PagingAlgorithm> algo) {
        std::cout << "--- handleMemoryAccess() quick test ---\n";
        const int NUM_FRAMES = 3;
        const int TLB_CAPACITY = 2;
        const int NUM_VIRTUAL_PAGES_PROCESS1 = 10;

        Simulation simulation(NUM_FRAMES, std::move(algo), TLB_CAPACITY);
        Process process1(1, NUM_VIRTUAL_PAGES_PROCESS1);
        simulation.setCurrentProcess(&process1);

        simulation.handleMemoryAccess({0});       // PF
        simulation.handleMemoryAccess({0});       // TLB hit
        simulation.handleMemoryAccess({1});       // PF
        simulation.handleMemoryAccess({2});       // PF
        simulation.handleMemoryAccess({2, true}); // Write hit
        simulation.printStatistics();
    }
};

#endif // HANDLEMEMORYACCESSTEST_H
