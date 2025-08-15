/**
* @file SecondChanceAlgorithm.cpp
 * @brief Implementation of Second-Chance replacement.
 */
#include "core/algorithms/SecondChanceAlgorithm.h"

void SecondChanceAlgorithm::memoryAccess(int pageId) {
    auto it = pageMap.find(pageId);
    if (it != pageMap.end()) it->second->referenced = true;
}

int SecondChanceAlgorithm::selectVictimPage() {
    if (clockList.empty()) throw std::logic_error("SecondChance: empty clock");
    while (true) {
        auto entry = clockList.front();
        clockList.pop_front();
        if (entry.referenced) {
            entry.referenced = false;
            clockList.push_back(entry);
            pageMap[entry.pageId] = std::prev(clockList.end());
        } else {
            pageMap.erase(entry.pageId);
            return entry.frameIndex;
        }
    }
}

void SecondChanceAlgorithm::pageLoaded(int pageId, int frameIndex) {
    Entry e{pageId, frameIndex, true}; // Newly loaded page is referenced once.
    clockList.push_back(e);
    pageMap[pageId] = std::prev(clockList.end());
}
