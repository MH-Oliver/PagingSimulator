/**
* @file NFUNoAgingAlgorithm.cpp
 * @brief Implementation of NFU without aging.
 */
#include "core/algorithms/NFUNoAgingAlgorithm.h"

void NFUNoAgingAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) ++(it->second.counter);
}

int NFUNoAgingAlgorithm::selectVictimPage() {
    if (table.empty()) throw std::logic_error("NFU(no aging): empty table");
    unsigned int minCount = std::numeric_limits<unsigned int>::max();
    int victimFrame = -1, victimPage = -1;
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

void NFUNoAgingAlgorithm::pageLoaded(int pageId, int frameIndex) {
    table[pageId] = Info{frameIndex, 0};
}
