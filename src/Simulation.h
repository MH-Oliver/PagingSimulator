#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>

#include "core/MemoryAccessEvent.h"
#include "core/CoreStructs.h"
#include "core/PagingAlgorithm.h"

using namespace std;

class Simulation {
private:
    vector<PageFrame> mainMemory;
    vector<Process> processes;
    PagingAlgorithm* pagingAlgorithm;
    MMU mmu; // Aktuelle Prozess hier verwaltet

public:

    Simulation(int numFrames, PagingAlgorithm* pagingAlgorithm, int tlbCapacity)
        : pagingAlgorithm(pagingAlgorithm), mmu(tlbCapacity) {
        mainMemory.resize(numFrames);
        for (PageFrame& frame : mainMemory) {
            frame.pageId = -1; // Frame am Anfang leer
            frame.referencedBit = false;
            frame.dirtyBit = false;
        }
    }

    /**
     * Innerhalb dieser Methode wird der Hauptspeicher durchsucht und es wird
     * zwischen einem Page Hit und einem Page Fault unterschieden.
     *
     * Bei einem Page Hit wird das referencedBit des entsprechenden Seiten-Rahmen auf true gesetzt.
     *
     * @param event Beinhaltet den Index der gesuchten Seite
     */
    void handleMemoryAccess(MemoryAccessEvent* event);

    void setCurrentProcess(Process* process) {
        mmu.setCurrentProcess(process);
    }

    void deleteTlbEntry(int victim_frame_index) {
        mmu.tlb.deleteEntry(victim_frame_index);
    }

    int getTlbPageForFrame(int victim_frame_index) {
        return mmu.tlb.getPageForFrame(victim_frame_index);
    }

    void handlePageFault(int requested_page_id);

private:
    void addTlbEntry(int get_page_id, int frame_index) {
        mmu.tlb.addEntry(get_page_id, frame_index);
    }
};

#endif //SIMULATION_H
