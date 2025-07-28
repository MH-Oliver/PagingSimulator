//
// Created by Son on 28.07.25.
//

#include "core/algorithms/NRUAlgorithm.h"

NRUAlgorithm::NRUAlgorithm() = default;

void NRUAlgorithm::memoryAccess(int pageId) {
    auto it = table.find(pageId);
    if (it != table.end()) {
        it->second.referenced = true;
    }
}

int NRUAlgorithm::selectVictimPage() {
    std::vector<int> class0, class1;
    for (auto &p : table) {
        if (p.second.referenced) class1.push_back(p.first);
        else class0.push_back(p.first);
    }
    auto &candidates = class0.empty() ? class1 : class0;
    int choice = candidates[std::rand() % candidates.size()];
    int victimFrame = table[choice].frameIndex;
    table.erase(choice);
    for (auto &p : table) p.second.referenced = false;
    return victimFrame;
}

void NRUAlgorithm::pageLoaded(int pageId, int frameIndex) {
    table[pageId] = Info{frameIndex, false};
}