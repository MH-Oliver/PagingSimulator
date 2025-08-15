/**
* @file NFUAlgorithm.cpp
 * @brief Implementation of NFU with simple aging.
 */
#include "core/algorithms/NFUAlgorithm.h"

void NFUAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) { ++(it->second.counter); }
}

int NFUAlgorithm::selectVictimPage() {
    if (table.empty()) throw std::logic_error("NFU: empty table");
    // Simple aging step: right-shift counters on each selection.
    for (auto &kv : table) kv.second.counter >>= 1;

    unsigned int minCount = std::numeric_limits<unsigned int>::max();
    int victimFrame = -1;
    int victimPage  = -1;
    for (auto &kv : table) {
        if (kv.second.counter < minCount) {
            minCount = kv.second.counter;
            victimFrame = kv.second.frameIndex;
            victimPage = kv.first;
        }
    }
    if (victimPage != -1) table.erase(victimPage);
    return victimFrame;
}

void NFUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    table[pageId] = Info{frameIndex, 0};
}
