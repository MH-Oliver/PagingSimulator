/**
 * @file Simulation.h
 * @brief Paging simulator with a TLB and pluggable replacement algorithms.
 *
 * The simulator models a paged memory hierarchy with:
 *  - Physical frames (@ref PageFrame)
 *  - A per-process page table inside the @ref MMU
 *  - A simple TLB inside the @ref MMU
 *  - A replacement policy (@ref PagingAlgorithm)
 *
 * It collects simple metrics: total accesses, TLB hits/misses, page faults, and
 * average access time based on a minimal timing model.
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <vector>
#include <iostream>

#include "core/CoreStructs.h"        // PageFrame, Process, MMU, TLB
#include "core/PagingAlgorithm.h"    // base class for paging policies
#include "core/MemoryAccessEvent.h"  // MemoryAccessEvent(getPageId())

/**
 * @brief Simulation of a paged memory system with a TLB.
 */
class Simulation {
public:
    /**
     * @brief Point-in-time statistics snapshot.
     */
    struct Stats {
        unsigned long accesses        {0};  //!< Total memory accesses processed
        unsigned long tlbHits         {0};  //!< TLB hits
        unsigned long tlbMisses       {0};  //!< TLB misses
        unsigned long pageFaults      {0};  //!< Page faults
        double        avgAccessTimeUs {0};  //!< Average access time (microseconds)
        double        tlbHitRate      {0};  //!< TLB hit rate [0,1]
        double        pageFaultRate   {0};  //!< Page fault rate [0,1]
    };

    /**
     * @brief Construct a simulation.
     * @param numFrames   Number of physical frames.
     * @param algo        Replacement algorithm (owned by the simulation).
     * @param tlbCapacity TLB capacity (number of entries).
     */
    Simulation(int numFrames, std::unique_ptr<PagingAlgorithm> algo, int tlbCapacity);

    ~Simulation() = default;

    /**
     * @brief Handle a single memory access: TLB lookup, hit/miss path, page fault.
     * @param event Memory access event (uses @c getPageId()).
     */
    void handleMemoryAccess(const MemoryAccessEvent& event);

    /**
     * @brief Handle a page fault: find/evict frame, load page, update PTE/TLB.
     * @param requestedPageId Virtual page to map in.
     */
    void handlePageFault(int requestedPageId);

    /**
     * @brief Set the current process (TLB is cleared).
     * @param process Pointer to the running process (not owned).
     */
    void setCurrentProcess(Process* process) { mmu_.setCurrentProcess(process); }

    /**
     * @brief Print human-readable statistics to stdout.
     */
    void printStatistics() const;

    /**
     * @brief Return a statistics snapshot.
     */
    Stats stats() const;

    /// @name Read-only views (preferred for GUI)
    /// @{
    const std::vector<PageFrame>& mainMemoryView() const { return mainMemory_; }
    const MMU&                    mmuView()        const { return mmu_; }
    /// @}

    /// @deprecated Use @ref mainMemoryView instead.
    [[deprecated("Use mainMemoryView() instead")]]
    std::vector<PageFrame> getMain_memory() const { return mainMemory_; }

    /// @deprecated Use @ref mmuView instead.
    [[deprecated("Use mmuView() instead")]]
    MMU getMMU() const { return mmu_; }

private:
    // --- State ---
    std::vector<PageFrame>           mainMemory_;      //!< Physical frames
    std::unique_ptr<PagingAlgorithm> pagingAlgorithm_; //!< Replacement policy (owned)
    MMU                               mmu_;            //!< MMU with TLB + current process

    // --- Statistics ---
    unsigned long totalAccesses_  {0};
    unsigned long tlbHits_        {0};
    unsigned long tlbMisses_      {0};
    unsigned long pageFaults_     {0};
    double        totalAccessTime_{0.0};  //!< Accumulated time in microseconds

    // --- Timing model (microseconds) ---
    static constexpr double TLB_HIT_TIME       = 1.0;     //!< TLB lookup time
    static constexpr double MEMORY_ACCESS_TIME = 100.0;   //!< Memory access (page hit after TLB miss)
    static constexpr double PAGE_FAULT_TIME    = 10000.0; //!< Page fault penalty
};

#endif // SIMULATION_H
