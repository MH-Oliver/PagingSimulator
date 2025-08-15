/**
* @file LRUAlgorithm.cpp
 * @brief Implementation of LRU page replacement.
 */
#include "core/algorithms/LRUAlgorithm.h"
#include <stdexcept>
#include <limits>

LRUAlgorithm::LRUAlgorithm() : accessCounter(0) {}

void LRUAlgorithm::memoryAccess(int pageId) {
    ++accessCounter;
    auto it = table.find(pageId);
    if (it != table.end()) it->second.lastUse = accessCounter;
}

int LRUAlgorithm::selectVictimPage() {
    if (table.empty()) throw std::logic_error("LRU: empty table");
    long oldest = std::numeric_limits<long>::max();
    int victimFrame = -1;
    int victimPage  = -1;
    for (auto &kv : table) {
        if (kv.second.lastUse < oldest) {
            oldest = kv.second.lastUse;
            victimFrame = kv.second.frameIndex;
            victimPage = kv.first;
        }
    }
    if (victimPage != -1) table.erase(victimPage);
    return victimFrame;
}

void LRUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    ++accessCounter;
    table[pageId] = Info{frameIndex, accessCounter};
}
