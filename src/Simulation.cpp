/**
 * @file Simulation.cpp
 * @brief Implementation of the paging simulation core.
 */

#include "Simulation.h"
#include <sstream>
#include <utility>

Simulation::Simulation(int numFrames,
                       std::unique_ptr<PagingAlgorithm> algo,
                       int tlbCapacity)
    : pagingAlgorithm_(std::move(algo)), mmu_(tlbCapacity)
{
    mainMemory_.resize(numFrames);
    for (auto& f : mainMemory_) {
        f.pageId = -1;
        f.referencedBit = false;
        f.dirtyBit = false;
        f.lastAccessTime = 0;
        f.loadTime = 0;
        f.accessCounter = 0;
    }
}

void Simulation::handleMemoryAccess(const MemoryAccessEvent& event) {
    totalAccesses_++;
    double accessTime = 0.0;
    const int  pageId  = event.pageId();
    const bool isWrite = event.write();

    // Step header for UI
    ++stepCounter_;
    {
        std::ostringstream os;
        os << "--- Schritt " << stepCounter_ << " (" << (isWrite ? 'W' : 'R')
           << "): Zugriff auf virtuelle Seite " << pageId << " ---";
        log(os.str());
    }

    if (!mmu_.currentProcess) {
        log("! Kein aktiver Prozess gesetzt.");
        return;
    }

    // Bounds check for page table access
    auto& entries = mmu_.currentProcess->page_table.entries;
    if (pageId < 0 || pageId >= static_cast<int>(entries.size())) {
        std::ostringstream os;
        os << "! Ungültige Seite " << pageId
           << " (0.." << (static_cast<int>(entries.size()) - 1) << "). Zugriff ignoriert.";
        log(os.str());
        return;
    }

    // 1) TLB lookup
    int frameIndex = mmu_.tlb.lookup(pageId);
    if (frameIndex != -1) {
        // TLB-Hit
        tlbHits_++;
        accessTime += TLB_HIT_TIME;

        {
            std::ostringstream os;
            os << "> TLB-Hit: Seite " << pageId << " -> Rahmen " << frameIndex << ".";
            log(os.str());
        }

        auto& frame = mainMemory_[frameIndex];
        frame.referencedBit = true;
        if (isWrite) {
            frame.dirtyBit = true;
            pagingAlgorithm_->onWrite(pageId);
            std::ostringstream os; os << "> Schreibzugriff: Dirty-Bit von Rahmen " << frameIndex << " gesetzt.";
            log(os.str());
        }
        pagingAlgorithm_->memoryAccess(pageId);
    } else {
        // TLB-Miss
        tlbMisses_++;
        accessTime += TLB_HIT_TIME; // cost to probe the TLB even on miss
        {
            std::ostringstream os;
            os << "> TLB-Miss für Seite " << pageId << ".";
            log(os.str());
        }

        if (!entries[pageId].isPresent) {
            // Page Fault
            pageFaults_++;
            accessTime += PAGE_FAULT_TIME;
            {
                std::ostringstream os;
                os << "> Page-Fault für Seite " << pageId << " (nicht im Speicher).";
                log(os.str());
            }
            // *** FIX: pass write flag for correct signature ***
            handlePageFault(pageId, isWrite);
        } else {
            // Page Hit in RAM
            frameIndex = entries[pageId].frameIndex;
            accessTime += MEMORY_ACCESS_TIME;

            {
                std::ostringstream os;
                os << "> Page-Hit: Seite " << pageId << " in Rahmen " << frameIndex << ".";
                log(os.str());
            }

            auto& frame = mainMemory_[frameIndex];
            frame.referencedBit = true;
            if (isWrite) {
                frame.dirtyBit = true;
                pagingAlgorithm_->onWrite(pageId);
                std::ostringstream os; os << "> Schreibzugriff: Dirty-Bit von Rahmen " << frameIndex << " gesetzt.";
                log(os.str());
            }

            pagingAlgorithm_->memoryAccess(pageId);
            mmu_.tlb.addOrUpdate(pageId, frameIndex);
            {
                std::ostringstream os;
                os << "> TLB wird aktualisiert: Seite " << pageId << " -> Rahmen " << frameIndex << ".";
                log(os.str());
            }
        }
    }

    totalAccessTime_ += accessTime;
}

void Simulation::handlePageFault(int requestedPageId, bool writeAccess) {
    // 1) try find free frame
    int targetFrame = -1;
    for (int i = 0; i < static_cast<int>(mainMemory_.size()); ++i) {
        if (mainMemory_[i].pageId == -1) { targetFrame = i; break; }
    }

    if (targetFrame != -1) {
        std::ostringstream os;
        os << "! Physikalischer Speicher hat freien Rahmen " << targetFrame
           << ". Lade Seite " << requestedPageId << " in Rahmen " << targetFrame << ".";
        log(os.str());
    } else {
        // 2) no free frame → evict
        targetFrame = pagingAlgorithm_->selectVictimPage();
        const int oldPage = mainMemory_[targetFrame].pageId;

        {
            std::ostringstream os;
            os << "! Speicher voll. Ersetze Seite " << oldPage
               << " aus Rahmen " << targetFrame << " gemäß Algorithmus.";
            log(os.str());
        }

        // Invalidate old mapping + TLB
        if (oldPage != -1) {
            auto& entries = mmu_.currentProcess->page_table.entries;
            entries[oldPage].isPresent  = false;
            entries[oldPage].frameIndex = -1;
        }
        mmu_.tlb.deleteEntryByFrame(targetFrame);
        {
            std::ostringstream os;
            os << "> TLB: Eintrag für Rahmen " << targetFrame << " entfernt.";
            log(os.str());
        }
    }

    // 3) map new page
    auto& frame = mainMemory_[targetFrame];
    frame.pageId        = requestedPageId;
    frame.referencedBit = true;
    frame.dirtyBit      = false;

    auto& pte = mmu_.currentProcess->page_table.entries[requestedPageId];
    pte.frameIndex = targetFrame;
    pte.isPresent  = true;

    // if write access, mark dirty and notify algorithm
    if (writeAccess) {
        frame.dirtyBit = true;
        pagingAlgorithm_->onWrite(requestedPageId);
        std::ostringstream os; os << "> Schreibzugriff: Dirty-Bit von Rahmen " << targetFrame << " gesetzt.";
        log(os.str());
    }

    // Update TLB & policy
    mmu_.tlb.addOrUpdate(requestedPageId, targetFrame);
    {
        std::ostringstream os;
        os << "> TLB wird aktualisiert: Seite " << requestedPageId
           << " -> Rahmen " << targetFrame << ".";
        log(os.str());
    }
    pagingAlgorithm_->pageLoaded(requestedPageId, targetFrame);
}

void Simulation::printStatistics() const {
    auto s = stats();
    std::cout << "\n=== Stats ===\n"
              << "Accesses      : " << s.accesses << "\n"
              << "TLB hits/miss : " << s.tlbHits << " / " << s.tlbMisses
              << " (hit " << s.tlbHitRate*100.0 << "%)\n"
              << "Page faults   : " << s.pageFaults
              << " (rate " << s.pageFaultRate*100.0 << "%)\n"
              << "Avg time (us) : " << s.avgAccessTimeUs << "\n";
}

Simulation::Stats Simulation::stats() const {
    Stats s;
    s.accesses        = totalAccesses_;
    s.tlbHits         = tlbHits_;
    s.tlbMisses       = tlbMisses_;
    s.pageFaults      = pageFaults_;
    s.avgAccessTimeUs = (totalAccesses_ ? totalAccessTime_ / totalAccesses_ : 0.0);
    s.tlbHitRate      = (totalAccesses_ ? double(tlbHits_)     / totalAccesses_ : 0.0);
    s.pageFaultRate   = (totalAccesses_ ? double(pageFaults_)  / totalAccesses_ : 0.0);
    return s;
}
