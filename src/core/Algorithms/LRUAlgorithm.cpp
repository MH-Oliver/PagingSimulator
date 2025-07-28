//
// Created by Son on 28.07.25.
//

#include "core/algorithms/LRUAlgorithm.h"

LRUAlgorithm::LRUAlgorithm() : accessCounter(0) {}

void LRUAlgorithm::memoryAccess(int pageId) {
    ++accessCounter;
    auto it = table.find(pageId);
    if (it != table.end()) {
        it->second.lastUse = accessCounter;
    }
}

int LRUAlgorithm::selectVictimPage() {
    long oldest = accessCounter + 1;
    int victimFrame = 0;
    int victimPage = -1;
    for (auto &p : table) {
        if (p.second.lastUse < oldest) {
            oldest = p.second.lastUse;
            victimFrame = p.second.frameIndex;
            victimPage = p.first;
        }
    }
    if (victimPage != -1) table.erase(victimPage);
    return victimFrame;
}

void LRUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    ++accessCounter;
    table[pageId] = Info{frameIndex, accessCounter};
}