// src/core/CoreStructs.h
/**
 * @file CoreStructs.h
 * @brief Core data structures for frames, TLB, processes, and MMU.
 */
#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H

#include <vector>
#include <deque>
#include <cstdint>

/** @brief One physical memory frame. */
struct PageFrame {
    int        pageId{-1};          ///< Virtual page currently stored (-1 if empty).
    bool       dirtyBit{false};      ///< Page has been written.
    bool       referencedBit{false}; ///< Recently referenced.
    long long  lastAccessTime{0};    ///< Optional: last access timestamp.
    long long  loadTime{0};          ///< Optional: time loaded.
    int        accessCounter{0};     ///< Optional: number of accesses.
};

/** @brief One entry in the page table. */
struct PageTableEntry {
    bool isPresent{false}; ///< Present/valid bit.
    int  frameIndex{-1};   ///< Mapped physical frame (-1 if none).
};

/** @brief Page table for a process. */
struct PageTable {
    std::vector<PageTableEntry> entries; ///< Entries indexed by page ID.
    explicit PageTable(unsigned int numVirtualPages) : entries(numVirtualPages) {}
};

/** @brief One TLB entry (page -> frame). */
struct TLBEntry {
    int           page_index{-1};   ///< Virtual page.
    int           frame_index{-1};  ///< Physical frame.
    unsigned char frame_attributes{0}; ///< Optional attribute bits.
};

/**
 * @brief A simple FIFO TLB with capacity-bound storage.
 * @details Can be extended to LRU or other policies.
 */
struct TLB {
    std::deque<TLBEntry> entries;   ///< FIFO container (front = oldest).
    unsigned int         capacity{0}; ///< Maximum entries.

    explicit TLB(unsigned int cap) : capacity(cap) {}

    /** @brief Remove all entries. */
    void clear() { entries.clear(); }

    /**
     * @brief Look up a page in the TLB.
     * @param pageIndex Virtual page.
     * @return Frame index or -1 if not found.
     */
    int lookup(int pageIndex) const {
        for (const auto& e : entries) {
            if (e.page_index == pageIndex) return e.frame_index;
        }
        return -1;
    }

    /**
     * @brief Insert or update a TLB entry using FIFO replacement.
     * @param pageIndex Virtual page.
     * @param frameIndex Physical frame.
     */
    void addOrUpdate(int pageIndex, int frameIndex) {
        if (capacity == 0) return;
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->page_index == pageIndex) { entries.erase(it); break; }
        }
        if (entries.size() == capacity) entries.pop_front();
        entries.push_back(TLBEntry{pageIndex, frameIndex, 0});
    }

    /**
     * @brief Remove the entry that references a given frame, if any.
     * @param victim_frame_index Frame to remove.
     */
    void deleteEntryByFrame(int victim_frame_index) {
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->frame_index == victim_frame_index) { entries.erase(it); break; }
        }
    }

    /**
     * @brief Reverse lookup: get the page mapped to a frame.
     * @param frame_index Frame to check.
     * @return Page index or -1.
     */
    int getPageForFrame(int frame_index) const {
        for (const auto& e : entries) {
            if (e.frame_index == frame_index) return e.page_index;
        }
        return -1;
    }
};

/** @brief A minimal process model containing a page table. */
struct Process {
    unsigned char process_id{0}; ///< Process identifier.
    PageTable     page_table;    ///< Page table.

    Process(unsigned char id, unsigned int numVirtualPages)
      : process_id(id), page_table(numVirtualPages) {}
};

/** @brief Minimal MMU wrapper that holds a TLB and the current process. */
struct MMU {
    TLB       tlb;                       ///< Translation lookaside buffer.
    Process*  currentProcess{nullptr};   ///< Active process whose page table is consulted.

    explicit MMU(unsigned int tlbCapacity) : tlb(tlbCapacity) {}

    /**
     * @brief Change the current process and clear the TLB.
     * @param p Process pointer (no ownership).
     */
    void setCurrentProcess(Process* p) {
        currentProcess = p;
        tlb.clear();
    }
};

#endif // CORESTRUCTS_H
