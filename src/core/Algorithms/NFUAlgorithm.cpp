//
// Created by Son on 28.07.25.
//

#include "core/algorithms/NFUAlgorithm.h"
#include <climits>

NFUAlgorithm::NFUAlgorithm() = default;

void NFUAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) {
        it->second.counter++;
    }
}

int NFUAlgorithm::selectVictimPage() {
    for (auto &p : table) {
        p.second.counter >>= 1;
    }
    unsigned int minCount = UINT_MAX;
    int victimFrame = 0;
    int victimPage = -1;
    for (auto &p : table) {
        if (p.second.counter < minCount) {
            minCount = p.second.counter;
            victimFrame = p.second.frameIndex;
            victimPage = p.first;
        }
    }
    if (victimPage != -1) table.erase(victimPage);
    return victimFrame;
}

void NFUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    table[pageId] = Info{frameIndex, 0};
}
