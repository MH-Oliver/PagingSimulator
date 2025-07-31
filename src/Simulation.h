#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <iostream>
#include "core/CoreStructs.h"
#include "core/PagingAlgorithm.h"
#include "core/MemoryAccessEvent.h"
#include "des/EventQueue.h"

/**
 * Simulation der Speicherverwaltung mit Paging
 * Verwaltet Hauptspeicher, TLB und Paging-Algorithmus
 * Sammelt Statistik: Zugriffe, TLB-Hits/Misses, Page Faults, durchschnittliche Zugriffszeit
 */
class Simulation {
private:
    std::vector<PageFrame> mainMemory;  // Physische Frames
    std::vector<Process> processes;     // Prozesse
    PagingAlgorithm* pagingAlgorithm;   // Aktueller Paging-Algorithmus
    MMU mmu;                            // Memory Management Unit (inkl. TLB)

    // Statistikfelder
    unsigned long totalAccesses = 0;
    unsigned long tlbHits = 0;
    unsigned long tlbMisses = 0;
    unsigned long pageFaults = 0;
    double totalAccessTime = 0.0;

    // Simulierte Zugriffszeiten (in µs)
    static constexpr double TLB_HIT_TIME = 1.0;
    static constexpr double MEMORY_ACCESS_TIME = 100.0;
    static constexpr double PAGE_FAULT_TIME = 10000.0;

public:
    /**
     * Konstruktor: Initialisiert Hauptspeicher und TLB
     * @param numFrames   Anzahl der physischen Frames
     * @param algo        PagingAlgorithm-Instanz
     * @param tlbCapacity Kapazität der TLB
     */
    Simulation(int numFrames, PagingAlgorithm* algo, int tlbCapacity);
    ~Simulation();

    /**
     * Behandelt einen Speicherzugriff-Event.
     * Führt TLB-Lookup, Page Hit/Miss, Page Fault sowie Statistikaktualisierung durch.
     */
    void handleMemoryAccess(MemoryAccessEvent* event);

    /**
     * Behandelt einen Page Fault: Freien Frame suchen oder Opfer wählen, Seite laden, TLB aktualisieren.
     */
    void handlePageFault(int requested_page_id);

    // Setzt aktuellen Prozess (Wechsel) und leert TLB
    void setCurrentProcess(Process* process) { mmu.setCurrentProcess(process); }

    // Hilfsfunktionen zur TLB-Verwaltung
    void deleteTlbEntry(int frameIndex) { mmu.tlb.deleteEntry(frameIndex); }
    int getTlbPageForFrame(int frameIndex) { return mmu.tlb.getPageForFrame(frameIndex); }

    /**
     * Gibt am Ende der Simulation alle gesammelten Statistiken aus.
     * Zeigt Gesamtzugriffe, Page Faults, TLB Hits/Misses und mittlere Zugriffszeit.
     */
    void printStatistics() const;
};

#endif // SIMULATION_H
