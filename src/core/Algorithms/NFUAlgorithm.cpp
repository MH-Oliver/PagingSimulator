/**
* @file NFUAlgorithm.cpp
 * @brief Implementation of NFU with classical aging (MSB injection).
 */
#include "core/algorithms/NFUAlgorithm.h"

void NFUAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) {
        it->second.referenced = true; // mark referenced; age injection happens on aging step
    }
}

int NFUAlgorithm::selectVictimPage() {
    if (table.empty()) throw std::logic_error("NFU(aging): empty table");

    // Aging step: shift right and inject R into MSB, then clear R
    for (auto &kv : table) {
        auto &inf = kv.second;
        inf.age = static_cast<uint8_t>((inf.age >> 1) | (inf.referenced ? 0x80 : 0x00));
        inf.referenced = false;
    }

    // Pick the coldest (smallest age). Tie-breaking by first minimum encountered.
    uint8_t minAge = std::numeric_limits<uint8_t>::max();
    int victimPage  = -1;
    int victimFrame = -1;
    for (const auto &kv : table) {
        const auto &inf = kv.second;
        if (inf.age < minAge) {
            minAge = inf.age;
            victimPage  = kv.first;
            victimFrame = inf.frameIndex;
        }
    }

    if (victimPage != -1) table.erase(victimPage);
    return victimFrame;
}

void NFUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    // Fresh page starts cold (age=0). Since the faulting access is the first access,
    // we mark referenced=true; Simulation will also call memoryAccess() on the same step,
    // which keeps referenced=true and will lead to MSB injection on the next aging.
    table[pageId] = Info{frameIndex, /*age*/0u, /*referenced*/true};
}
