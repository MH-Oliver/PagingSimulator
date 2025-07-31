//
// Created by Son on 31.07.25.
//

#include "core/algorithms/NFUNoAgingAlgorithm.h"

void NFUNoAgingAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) {
        // Einfaches NFU ohne Aging: reiner Zähler
        ++(it->second.counter);
    }
}

int NFUNoAgingAlgorithm::selectVictimPage() {
    unsigned int minCount = UINT_MAX;
    int victimFrame = 0;
    int victimPage = -1;
    // Finde Seite mit minimalem Zähler
    for (auto &p : table) {
        if (p.second.counter < minCount) {
            minCount = p.second.counter;
            victimFrame = p.second.frameIndex;
            victimPage = p.first;
        }
    }
    if (victimPage != -1) {
        table.erase(victimPage);
    }
    return victimFrame;
}

void NFUNoAgingAlgorithm::pageLoaded(int pageId, int frameIndex) {
    // Neue Seite: Zähler auf 0
    table[pageId] = Info{frameIndex, 0};
}