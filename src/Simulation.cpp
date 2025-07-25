#include "Simulation.h"

#include <iostream>


void Simulation::handleMemoryAccess(MemoryAccessEvent* event) {
    if (!mmu.currentProcess) {
        cout << "Kein Process aktiv";
        return;
    }

    int requestedPageId = event->getPageId();
    cout << "Prozess " << (int)mmu.currentProcess->process_id << " Zugriff auf Seite: " << requestedPageId << endl;

    // 1. TLB prüfen (über die MMU)
    int frameIndex = mmu.tlb.lookup(requestedPageId);

    if (frameIndex != -1) { // TLB HIT: Frame wurde gefunden.
        cout << "  -> TLB Hit! Seite " << requestedPageId << " gefunden in Rahmen " << frameIndex << endl;
        mainMemory[frameIndex].referencedBit = true;
        pagingAlgorithm->memoryAccess(requestedPageId);
    } else { // TLB Miss
        cout << "  -> TLB Miss für Seite " << requestedPageId << endl;

        // 2. Seitentabelle des aktuellen Prozesses prüfen (über die MMU)
        if (mmu.currentProcess->page_table.entries.size() <= requestedPageId) {
            cerr << "Ungültiges Event";
            return;
        }
        // Sichergestellt: Seite existiert virtuell
        PageTableEntry* pte = &mmu.currentProcess->page_table.entries[requestedPageId];

        // Es muss nun geprüft werden, ob Seite auch im physischem Speicher
        if (pte->isPresent) { // Page Hit (nach TLB Miss)
            cout << "  -> Page Hit! (nach TLB Miss) Seite " << requestedPageId << " gefunden in Rahmen " << frameIndex <<endl;
            frameIndex = pte->frameIndex;

            mainMemory[frameIndex].referencedBit = true;
            pagingAlgorithm->memoryAccess(requestedPageId);
            mmu.tlb.addEntry(requestedPageId, frameIndex);

        } else { // Page Fault (Seite nicht im physischem Speicher)
            std::cout << "  -> Page Fault! Seite " << requestedPageId << " nicht im Hauptspeicher." << std::endl;
            this->handlePageFault(requestedPageId);
        }
    }
}

/**
 * Bestimmung einer alten Seite, um die neue Seite in den physichen Speicher zu laden, und die alte zu entfernen.
 * @param requested_page_id Seite die in den physichen Speicher geladen werden soll.
 */
void Simulation::handlePageFault(int requested_page_id) {
    // Suche, ob noch freier Platz im Speicher
    int victimFrameIndex = -1;
    for(int i = 0; i < mainMemory.size(); ++i) {
        if (mainMemory[i].pageId == -1) {
            victimFrameIndex = i;
            break;
        }
    }
    if (victimFrameIndex == -1) {
        // Zu überschreibenden Frame finden
        victimFrameIndex = pagingAlgorithm->selectVictimPage();
        cout << "  [Simulation] Paging-Algorithmus wählte Frame " << victimFrameIndex << " als Opfer." << endl;
    }

    // Ueberschreibe den Frame mit der neuen Seite
    PageFrame& victimFrame = mainMemory[victimFrameIndex];
    victimFrame.pageId = requested_page_id;
    victimFrame.dirtyBit = false;
    victimFrame.referencedBit = true;

    addTlbEntry(requested_page_id, victimFrameIndex);

    // Informiere den Algorithmus über die neue Seite
    pagingAlgorithm->pageLoaded(requested_page_id, victimFrameIndex);


}

