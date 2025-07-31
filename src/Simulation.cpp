#include "Simulation.h"

Simulation::Simulation(int numFrames, PagingAlgorithm* algo, int tlbCapacity)
    : pagingAlgorithm(algo), mmu(tlbCapacity) {
    // Hauptspeicher initialisieren: Frames leer
    mainMemory.resize(numFrames);
    for (auto& frame : mainMemory) {
        frame.pageId = -1;
        frame.referencedBit = false;
        frame.dirtyBit = false;
    }
}

Simulation::~Simulation() {
    // Ressourcenfreigabe falls nötig
}

void Simulation::handleMemoryAccess(MemoryAccessEvent* event) {
    totalAccesses++;
    double accessTime = 0.0;
    int pageId = event->getPageId();
    std::cout << "Prozess " << (int)mmu.currentProcess->process_id
              << " Zugriff auf Seite: " << pageId << std::endl;

    // 1) TLB-Lookup
    int frameIndex = mmu.tlb.lookup(pageId);
    if (frameIndex != -1) {
        // TLB-Hit
        tlbHits++;
        accessTime += TLB_HIT_TIME;
        std::cout << "  -> TLB Hit: Rahmen " << frameIndex << std::endl;
        mainMemory[frameIndex].referencedBit = true;
        pagingAlgorithm->memoryAccess(pageId);
    } else {
        // TLB-Miss
        tlbMisses++;
        accessTime += TLB_HIT_TIME;
        std::cout << "  -> TLB Miss für Seite " << pageId << std::endl;
        auto& entries = mmu.currentProcess->page_table.entries;
        if (!entries[pageId].isPresent) {
            // Page Fault
            pageFaults++;
            accessTime += PAGE_FAULT_TIME;
            std::cout << "  -> Page Fault! Seite " << pageId << " nicht im Hauptspeicher." << std::endl;
            handlePageFault(pageId);
        } else {
            // Page Hit
            frameIndex = entries[pageId].frameIndex;
            accessTime += MEMORY_ACCESS_TIME;
            std::cout << "  -> Page Hit: Rahmen " << frameIndex << std::endl;
            mainMemory[frameIndex].referencedBit = true;
            pagingAlgorithm->memoryAccess(pageId);
            mmu.tlb.addEntry(pageId, frameIndex);
        }
    }
    totalAccessTime += accessTime;
}

void Simulation::handlePageFault(int requested_page_id) {
    // Freien Frame suchen
    int victim = -1;
    for (int i = 0; i < mainMemory.size(); ++i) {
        if (mainMemory[i].pageId == -1) { victim = i; break; }
    }
    // Oder Opfer durch Algorithmus wählen
    if (victim == -1) {
        victim = pagingAlgorithm->selectVictimPage();
        std::cout << "  [Simulation] Opfer-Rahmen: " << victim << std::endl;
        int oldPage = getTlbPageForFrame(victim);
        if (oldPage != -1) deleteTlbEntry(victim);
        mmu.currentProcess->page_table.entries[oldPage].isPresent = false;
    }
    // Neue Seite laden
    auto& frame = mainMemory[victim];
    frame.pageId = requested_page_id;
    frame.dirtyBit = false;
    frame.referencedBit = true;
    auto& pte = mmu.currentProcess->page_table.entries[requested_page_id];
    pte.frameIndex = victim;
    pte.isPresent  = true;
    mmu.tlb.addEntry(requested_page_id, victim);
    pagingAlgorithm->pageLoaded(requested_page_id, victim);
}

void Simulation::printStatistics() const {
    std::cout << "\n=== Simulationsstatistik ===\n";
    std::cout << "Gesamte Zugriffe: " << totalAccesses << std::endl;
    std::cout << "Page Faults: " << pageFaults << std::endl;
    std::cout << "TLB Hits: " << tlbHits << " / Misses: " << tlbMisses << std::endl;
    double avg = totalAccesses ? totalAccessTime / totalAccesses : 0.0;
    std::cout << "Durchschnittliche Zugriffszeit (\u00B5s): " << avg << std::endl;
}