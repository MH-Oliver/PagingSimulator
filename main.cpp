#include "src/core/DummyPagingAlgorithm.h"
#include "test/DesTest.h"
#include "test/HandleMemoryAccessTest.h"
#include "core/algorithms/FIFOAlgorithm.h"
#include "core/algorithms/SecondChanceAlgorithm.h"
#include "core/algorithms/LRUAlgorithm.h"
#include "core/algorithms/NRUAlgorithm.h"
#include "core/algorithms/NFUAlgorithm.h"
#include "core/algorithms/NFUNoAgingAlgorithm.h"
#include <iostream>
#include "des/EventQueue.h"
#include "Simulation.h"
#include "TraceLoader.h"
int main() {
     DesTest::executeTests();
     HandleMemoryAccessTest::executeTests(new FIFOAlgorithm());
    //HandleMemoryAccessTest::executeTests(new SecondChanceAlgorithm());
    //HandleMemoryAccessTest::executeTests(new LRUAlgorithm());
    //HandleMemoryAccessTest::executeTests(new NRUAlgorithm());
    //HandleMemoryAccessTest::executeTests(new NFUAlgorithm());
    //HandleMemoryAccessTest::executeTests(new NFUNoAgingAlgorithm());
    const int NUM_FRAMES = 4;
    const int TLB_CAPACITY = 2;
    const int NUM_VIRTUAL_PAGES = 10;
    const std::string TRACE_FILE = "resources/trace.txt";

    // 1) DES initialisieren
    EventQueue q;
    auto* algo = new FIFOAlgorithm();
    auto* sim  = new Simulation(NUM_FRAMES, algo, TLB_CAPACITY);
    Process p1(1, NUM_VIRTUAL_PAGES);
    sim->setCurrentProcess(&p1);

    // 2) Trace laden und planen
    loadTrace(TRACE_FILE, q, sim, 1.0, 1.0);

    // 3) Simulation ausführen
    q.run();
    sim->printStatistics();

    delete sim;
    delete algo;
    return 0;
}