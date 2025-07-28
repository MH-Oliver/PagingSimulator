//
// Created by Son on 28.07.25.
//

#ifndef CORE_ALGORITHMS_FIFOALGORITHM_H
#define CORE_ALGORITHMS_FIFOALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <queue>

class FIFOAlgorithm : public PagingAlgorithm {
public:
    FIFOAlgorithm();
    ~FIFOAlgorithm() override = default;
    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;
private:
    std::queue<int> frameQueue;
};

#endif // CORE_ALGORITHMS_FIFOALGORITHM_H
