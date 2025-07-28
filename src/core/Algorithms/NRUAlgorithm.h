//
// Created by Son on 28.07.25.
//

#ifndef CORE_ALGORITHMS_NRUALGORITHM_H
#define CORE_ALGORITHMS_NRUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <vector>
#include <cstdlib>

class NRUAlgorithm : public PagingAlgorithm {
public:
    NRUAlgorithm();
    ~NRUAlgorithm() override = default;
    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;
private:
    struct Info { int frameIndex; bool referenced; };
    std::unordered_map<int, Info> table;
};

#endif // CORE_ALGORITHMS_NRUALGORITHM_H