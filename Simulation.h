#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>

#include "core/MemoryAccessEvent.h"
#include "core/CoreStructs.h"
#include "core/PagingAlgorithm.h"

using namespace std;

class Simulation {
public:
    vector<PageFrame> mainMemory;
    vector<Process> processes;
    PagingAlgorithm* pagingAlgorithm;
    MMU mmu;

    Process* currentProcess; // Aktuell ausgeführter Process

    void handlePageFault(int requested_page_id);

    /**
     * Wird aufgerufen, um eine Seite des laufenden Processes aus dem Hauptspeicher abzurufen,
     * bzw. um zu prüfen, ob der Prozess überhaupt im physischem Speicher vorliegt.
     *
     * - Lässt sich der Seite über das TLB ein Rahmen zuordnen, kann dieser sofort abgerufen werden (TLB Hit).
     *   Falls nicht, wird die Seitentabelle des aktuellen Prozesses geprüft.
     *     - Liegt hierbei ein Verweis auf den entsprechenden Rahhmen vor, wird auf diesen zugeriffen.
     *     - Falls nicht, wird ein Page-Fault behandelt-
     *
     * @param event Beinhaltet den Index der gesuchten Seite
     *
     * TODO Es sollte noch in der Main ein Test hierfür geschrieben werden
     */
    void handleMemoryAccess(MemoryAccessEvent event);
};

#endif //SIMULATION_H
