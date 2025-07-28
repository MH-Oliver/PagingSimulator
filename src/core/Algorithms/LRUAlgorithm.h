//
// Created by Son on 28.07.25.
//

#ifndef CORE_ALGORITHMS_LRUALGORITHM_H
#define CORE_ALGORITHMS_LRUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>

class LRUAlgorithm : public PagingAlgorithm {
public:
    LRUAlgorithm();
    ~LRUAlgorithm() override = default;
    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;
private:
    long accessCounter;
    struct Info { int frameIndex; long lastUse; };
    std::unordered_map<int, Info> table;
};

#endif // CORE_ALGORITHMS_LRUALGORITHM_H
