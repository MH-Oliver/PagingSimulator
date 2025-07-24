#include "Simulation.h"

#include <iostream>


void Simulation::handleMemoryAccess(MemoryAccessEvent event) {
    if (!currentProcess) {
        cout << "Kein Process aktiv";
        return;
    }

    int requestedPageId = event.getPageId();
    cout << "Prozess " << (int)currentProcess->process_id << " Zugriff auf Seite: " << requestedPageId << endl;

    // 1. TLB prüfen (über die MMU)
    int frameIndex = mmu.tlb.lookup(requestedPageId);

    if (frameIndex != -1) { // TLB HIT: Frame wurde gefunden.
        cout << "  -> TLB Hit! Seite " << requestedPageId << " gefunden in Rahmen " << frameIndex << endl;
        mainMemory[frameIndex].referencedBit = true;
        //TODO pagingAlgorithm->memoryAccess(requestedPageId);
    } else { // TLB Miss
        cout << "  -> TLB Miss für Seite " << requestedPageId << endl;

        // 2. Seitentabelle des aktuellen Prozesses prüfen (über die MMU)
        if (currentProcess->page_table.entries.size() <= requestedPageId) {
            cerr << "Ungültiges Event";
            return;
        }
        // Sichergestellt: Seite existiert virtuell
        PageTableEntry* pte = &currentProcess->page_table.entries[requestedPageId];

        // Es muss nun geprüft werden, ob Seite auch im physischem Speicher
        if (pte->isPresent) { // Page Hit (nach TLB Miss)
            cout << "  -> Page Hit! (nach TLB Miss) Seite " << requestedPageId << " gefunden in Rahmen " << frameIndex <<endl;
            frameIndex = pte->frameIndex;

            mainMemory[frameIndex].referencedBit = true;
            //TODO pagingAlgorithm->memoryAccess(requestedPageId);
            mmu.tlb.addEntry(requestedPageId, frameIndex);

        } else { // Page Fault (Seite nicht im physischem Speicher)
            std::cout << "  -> Page Fault! Seite " << requestedPageId << " nicht im Hauptspeicher." << std::endl;
            this->handlePageFault(requestedPageId);
        }
    }
}

void Simulation::handlePageFault(int requested_page_id) {
    // TODO
}

