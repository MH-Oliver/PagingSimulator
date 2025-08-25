/**
* @file NRUAlgorithm.cpp
 * @brief Implementation of NRU page replacement.
 */
#include "core/algorithms/NRUAlgorithm.h"

NRUAlgorithm::NRUAlgorithm(uint32_t seed) : rng(seed) {}

void NRUAlgorithm::memoryAccess(int pageId) {
    ++accessCount;
    auto it = table.find(pageId);
    if (it != table.end()) it->second.referenced = true;

    if (accessCount % resetPeriod == 0) {
        for (auto &kv : table) kv.second.referenced = false;
    }
}

void NRUAlgorithm::onWrite(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) it->second.dirty = true;
}

int NRUAlgorithm::pickRandom(const std::vector<int>& v) {
    if (v.empty()) return -1;
    std::uniform_int_distribution<> d(0, static_cast<int>(v.size())-1);
    return v[d(rng)];
}

int NRUAlgorithm::selectVictimPage() {
    if (table.empty()) throw std::logic_error("NRU: empty table");

    std::vector<int> c00, c01, c10, c11;
    for (auto &kv : table) {
        const auto &inf = kv.second;
        if (!inf.referenced && !inf.dirty) c00.push_back(kv.first);
        else if (!inf.referenced && inf.dirty) c01.push_back(kv.first);
        else if (inf.referenced && !inf.dirty) c10.push_back(kv.first);
        else c11.push_back(kv.first);
    }
    int victimPage = pickRandom(c00);
    if (victimPage == -1) victimPage = pickRandom(c01);
    if (victimPage == -1) victimPage = pickRandom(c10);
    if (victimPage == -1) victimPage = pickRandom(c11);

    int victimFrame = table[victimPage].frameIndex;
    table.erase(victimPage);

    for (auto &kv : table) {
        kv.second.referenced = false;
    }

    return victimFrame;
}

void NRUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    table[pageId] = Info{frameIndex, true, false};
}
