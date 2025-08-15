/**
 * @file Simulation.cpp
 * @brief Implementation of the paging simulation.
 */

#include "Simulation.h"

Simulation::Simulation(int numFrames,
                       std::unique_ptr<PagingAlgorithm> algo,
                       int tlbCapacity)
    : pagingAlgorithm_(std::move(algo)), mmu_(tlbCapacity)
{
    // Initialize physical frames as empty.
    mainMemory_.resize(numFrames);
    for (auto& f : mainMemory_) {
        f.pageId         = -1;
        f.referencedBit  = false;
        f.dirtyBit       = false;
        f.lastAccessTime = 0;
        f.loadTime       = 0;
        f.accessCounter  = 0;
    }
}

void Simulation::handleMemoryAccess(const MemoryAccessEvent& event) {
    totalAccesses_++;
    double accessTime = 0.0;

    const int pageId = event.pageId(); // ✅ 최신 API

    if (!mmu_.currentProcess) {
        std::cerr << "[Simulation] No current process set!\n";
        return;
    }

    // 1) TLB lookup
    int frameIndex = mmu_.tlb.lookup(pageId);
    if (frameIndex != -1) {
        // TLB hit
        tlbHits_++;
        accessTime += TLB_HIT_TIME;

        auto& frame = mainMemory_[frameIndex];
        frame.referencedBit = true;

        pagingAlgorithm_->memoryAccess(pageId);
    } else {
        // TLB miss
        tlbMisses_++;
        accessTime += TLB_HIT_TIME;

        auto& entries = mmu_.currentProcess->page_table.entries;
        if (!entries[pageId].isPresent) {
            // Page fault path
            pageFaults_++;
            accessTime += PAGE_FAULT_TIME;
            handlePageFault(pageId);
        } else {
            // Page hit in main memory
            frameIndex = entries[pageId].frameIndex;
            accessTime += MEMORY_ACCESS_TIME;

            auto& frame = mainMemory_[frameIndex];
            frame.referencedBit = true;

            pagingAlgorithm_->memoryAccess(pageId);
            mmu_.tlb.addOrUpdate(pageId, frameIndex); // ✅ 최신 API
        }
    }

    totalAccessTime_ += accessTime;
}

void Simulation::handlePageFault(int requestedPageId) {
    // 1) Find a free frame (first-fit)
    int targetFrame = -1;
    for (int i = 0; i < static_cast<int>(mainMemory_.size()); ++i) {
        if (mainMemory_[i].pageId == -1) { targetFrame = i; break; }
    }

    // 2) If none, ask the replacement policy for a victim
    if (targetFrame == -1) {
        targetFrame = pagingAlgorithm_->selectVictimPage();

        // Invalidate old PTE and remove TLB entry by frame index.
        const int oldPage = mainMemory_[targetFrame].pageId; // evicted page
        if (oldPage != -1) {
            auto& entries = mmu_.currentProcess->page_table.entries;
            entries[oldPage].isPresent  = false;
            entries[oldPage].frameIndex = -1;
            mmu_.tlb.deleteEntryByFrame(targetFrame); // ✅ 최신 API
        }
    }

    // 3) Map the requested page into the chosen frame
    auto& frame = mainMemory_[targetFrame];
    frame.pageId        = requestedPageId;
    frame.dirtyBit      = false;
    frame.referencedBit = true;

    auto& pte = mmu_.currentProcess->page_table.entries[requestedPageId];
    pte.frameIndex = targetFrame;
    pte.isPresent  = true;

    // Update TLB and notify policy
    mmu_.tlb.addOrUpdate(requestedPageId, targetFrame); // ✅ 최신 API
    pagingAlgorithm_->pageLoaded(requestedPageId, targetFrame);

    // Count the initial access that caused the fault
    pagingAlgorithm_->memoryAccess(requestedPageId);
}

void Simulation::printStatistics() const {
    std::cout << "\n=== Simulation Stats ===\n";
    std::cout << "Accesses: "    << totalAccesses_ << '\n';
    std::cout << "Page Faults: " << pageFaults_    << '\n';
    std::cout << "TLB Hits: "    << tlbHits_
              << " / Misses: "   << tlbMisses_     << '\n';

    const double avg = totalAccesses_
        ? (totalAccessTime_ / static_cast<double>(totalAccesses_)) : 0.0;
    const double hitRate = (tlbHits_ + tlbMisses_)
        ? (static_cast<double>(tlbHits_) / (tlbHits_ + tlbMisses_)) : 0.0;
    const double pfRate  = totalAccesses_
        ? (static_cast<double>(pageFaults_) / totalAccesses_) : 0.0;

    std::cout << "Avg access time (us): " << avg     << '\n';
    std::cout << "TLB hit rate: "         << hitRate << '\n';
    std::cout << "Page fault rate: "      << pfRate  << '\n';
}

Simulation::Stats Simulation::stats() const {
    Stats s;
    s.accesses        = totalAccesses_;
    s.tlbHits         = tlbHits_;
    s.tlbMisses       = tlbMisses_;
    s.pageFaults      = pageFaults_;
    s.avgAccessTimeUs = totalAccesses_
        ? (totalAccessTime_ / static_cast<double>(totalAccesses_)) : 0.0;
    s.tlbHitRate      = (tlbHits_ + tlbMisses_)
        ? (static_cast<double>(tlbHits_) / (tlbHits_ + tlbMisses_)) : 0.0;
    s.pageFaultRate   = totalAccesses_
        ? (static_cast<double>(pageFaults_) / totalAccesses_) : 0.0;
    return s;
}
