//
// Created by Son on 28.07.25.
//

#include "core/algorithms/SecondChanceAlgorithm.h"

SecondChanceAlgorithm::SecondChanceAlgorithm() = default;

void SecondChanceAlgorithm::memoryAccess(int pageId) {
    auto it = pageMap.find(pageId);
    if (it != pageMap.end()) {
        it->second->referenced = true;
    }
}

int SecondChanceAlgorithm::selectVictimPage() {
    while (true) {
        Entry entry = clockList.front();
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
    Entry e{pageId, frameIndex, false};
    clockList.push_back(e);
    pageMap[pageId] = std::prev(clockList.end());
}
