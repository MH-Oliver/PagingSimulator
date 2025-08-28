#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <vector>
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
     * @brief Handles a single memory access event.
     * @details
     * Simulates the entire process of a memory access, including TLB lookup,
     * page table lookup, and the handling of page faults.
     *
     * The process is as follows:
     * 1.  **TLB Lookup**: On a hit (TLB-Hit), the frame's metadata is updated.
     * 2.  **Page Table Lookup**: On a TLB miss, the page table is checked.
     * 3.  **Page Fault**: If the page is not in main memory, a page fault is triggered.
     * 4.  **Page Hit**: If the page is in main memory, the access is simulated and the TLB is updated.
     *
     * The method also updates the relevant counters for the simulation statistics.
     *
     * @param event The event containing the target data for the memory access (virtual page number and access type R/W).
     */
    void handleMemoryAccess(const MemoryAccessEvent& event);

    /**
     * @brief Handles a page fault by loading a page into a physical frame.
     * @details Finds a free frame or evicts a victim page via the paging algorithm.
     * It then loads the requested page and updates the page table and TLB.
     * @param requestedPageId The virtual page ID that caused the fault.
     * @param writeAccess     True if the fault was from a write operation.
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