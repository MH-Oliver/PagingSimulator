#ifndef HANDLEMEMORYACCESSTEST_H
#define HANDLEMEMORYACCESSTEST_H

#include "Simulation.h"
#include "des/Des.h"

class HandleMemoryAccessTest {
public:
    static void executeTests(PagingAlgorithm* pagingAlgorithm) {
        cout << "--- Test von handleMemoryAccess() ---" << endl;

        const int NUM_FRAMES = 3;
        const int TLB_CAPACITY = 2;
        const int NUM_VIRTUAL_PAGES_PROCESS1 = 10;

        Simulation simulation(NUM_FRAMES, pagingAlgorithm, TLB_CAPACITY);
        Process process1(1, NUM_VIRTUAL_PAGES_PROCESS1);
        simulation.setCurrentProcess(&process1);

        std::cout << "\n--- Testfall 1: Erster Zugriff (Page Fault) ---" << std::endl;
        MemoryAccessEvent event1(0);
        simulation.handleMemoryAccess(&event1);

        std::cout << "\n--- Testfall 2: Zugriff auf gleiche Seite (Page Hit / TLB Hit erwartet) ---" << std::endl;
        // Seite 0 ist jetzt in Frame 0 und sollte auch in der TLB sein.
        MemoryAccessEvent event2(0);
        simulation.handleMemoryAccess(&event2);

        std::cout << "\n--- Testfall 3: Zugriff auf neue Seite (Page Fault, füllt TLB) ---" << std::endl;
        // Seite 1 ist nicht im Speicher.
        MemoryAccessEvent event3(1);
        simulation.handleMemoryAccess(&event3);

        std::cout << "\n--- Testfall 4.1: Zugriff auf dritte Seite (Page Fault, TLB überläuft) ---" << std::endl;
        MemoryAccessEvent event4(2);
        simulation.handleMemoryAccess(&event4);
        std::cout << "\n--- Testfall 4.2: Erneute Zugriff auf gleiche Seite. Page Hit (nach TLB Miss) ---" << std::endl;
        simulation.handleMemoryAccess(&event4);
        std::cout << "\n--- Testfall 4.3: Zugriff auf erste Seite. (Page Fault, TLB überläuft) ---" << std::endl;
        simulation.handleMemoryAccess(&event1);
    }
};



#endif //HANDLEMEMORYACCESSTEST_H
