//
// Created by Son on 28.07.25.
//

#ifndef CORE_ALGORITHMS_NFUALGORITHM_H
#define CORE_ALGORITHMS_NFUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>

class NFUAlgorithm : public PagingAlgorithm {
public:
    NFUAlgorithm();
    ~NFUAlgorithm() override = default;
    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;
private:
    struct Info { int frameIndex; unsigned int counter; };
    std::unordered_map<int, Info> table;
};

#endif // CORE_ALGORITHMS_NFUALGORITHM_H
