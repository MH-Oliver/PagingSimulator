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
#include <functional>   ///< Logger callback
#include <string>

#include "core/CoreStructs.h"
#include "core/PagingAlgorithm.h"
#include "core/MemoryAccessEvent.h"

/**
 * @class Simulation
 * @brief Paging simulation engine.
 *
 * - Inject any replacement algorithm implementing @ref PagingAlgorithm.
 * - Keeps track of TLB hits/misses, page faults and average access time.
 * - Can forward step-by-step messages to a UI through a logger callback.
 */
class Simulation {
public:
    /**
     * @brief Callback type for one-line log messages.
     * @details Each call receives a plain-text, human-readable message.
     */
    using Logger = std::function<void(const std::string&)>;

    /**
     * @brief Set or replace the external logger.
     * @param cb Callback to receive log lines (moved). If empty, messages are dropped.
     */
    void setLogger(Logger cb) { logger_ = std::move(cb); }


    /**
     * @brief Aggregated simulation statistics.
     */
    struct Stats {
        unsigned long accesses{0};        ///< Total memory accesses.
        unsigned long tlbHits{0};         ///< TLB hits.
        unsigned long tlbMisses{0};       ///< TLB misses.
        unsigned long pageFaults{0};      ///< Page faults.
        double        avgAccessTimeUs{0}; ///< Average time per access (microseconds).
        double        tlbHitRate{0};      ///< TLB hit rate   in [0,1].
        double        pageFaultRate{0};   ///< Page fault rate in [0,1].
    };

    /**
     * @brief Construct the simulation.
     * @param numFrames    Number of physical frames.
     * @param algo         Replacement algorithm (ownership transferred).
     * @param tlbCapacity  TLB capacity (number of entries).
     */
    Simulation(int numFrames, std::unique_ptr<PagingAlgorithm> algo, int tlbCapacity);
    ~Simulation() = default;

    /**
     * @brief Handle a single memory access event.
     * @param event Accessed virtual page and optional write flag.
     */
    void handleMemoryAccess(const MemoryAccessEvent& event);

    /**
     * @brief Handle a page fault for a requested page.
     * @param requestedPageId Virtual page to load.
     * @param writeAccess     True if the triggering access was a write.
     */
    void handlePageFault(int requestedPageId, bool writeAccess);

    /**
     * @brief Set the current process (also clears the TLB).
     * @param process Non-owning pointer to the active process.
     */
    void setCurrentProcess(Process* process) { mmu_.setCurrentProcess(process); }

    /**
     * @brief Print statistics to std::cout (CLI demo helper).
     */
    void printStatistics() const;

    /**
     * @brief Return a snapshot of the current statistics.
     */
    Stats stats() const;

    /**
     * @brief Read-only view of the physical memory frames.
     */
    const std::vector<PageFrame>& mainMemoryView() const { return mainMemory_; }

    /**
     * @brief Read-only view of the MMU (including the TLB).
     */
    const MMU& mmuView() const { return mmu_; }

    /**
     * @deprecated Use @ref mainMemoryView().
     */
    [[deprecated("Use mainMemoryView() instead")]]
    std::vector<PageFrame> getMain_memory() const { return mainMemory_; }

    /**
     * @deprecated Use @ref mmuView().
     */
    [[deprecated("Use mmuView() instead")]]
    MMU getMMU() const { return mmu_; }

private:
    /**
     * @brief Emit a log line if a logger is installed.
     * @param msg Text to forward to the external logger.
     */
    void log(const std::string& msg) { if (logger_) logger_(msg); }

    std::vector<PageFrame>           mainMemory_;        ///< Physical memory frames.
    std::unique_ptr<PagingAlgorithm> pagingAlgorithm_;   ///< Replacement policy.
    MMU                              mmu_;               ///< MMU with a FIFO TLB.

    // Counters / accumulation
    unsigned long totalAccesses_{0};
    unsigned long tlbHits_{0};
    unsigned long tlbMisses_{0};
    unsigned long pageFaults_{0};
    double        totalAccessTime_{0.0};

    // Step counter for UI headers ("Schritt N").
    unsigned long stepCounter_{0};

    // Optional external logger injected by the UI.
    Logger logger_;

    // Time constants (arbitrary units; treated as microseconds).
    static constexpr double TLB_HIT_TIME       = 1.0;
    static constexpr double MEMORY_ACCESS_TIME = 100.0;
    static constexpr double PAGE_FAULT_TIME    = 10000.0;
};

#endif // SIMULATION_H