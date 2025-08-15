/**
* @file FIFOAlgorithm.h
 * @brief First-In-First-Out page replacement algorithm.
 */
#ifndef CORE_ALGORITHMS_FIFOALGORITHM_H
#define CORE_ALGORITHMS_FIFOALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <queue>
#include <stdexcept>

/**
 * @brief FIFO replacement: evict the frame that was filled earliest.
 */
class FIFOAlgorithm : public PagingAlgorithm {
public:
    FIFOAlgorithm() = default;
    ~FIFOAlgorithm() override = default;

    void memoryAccess(int /*pageId*/) override {}
    int  selectVictimPage() override;
    void pageLoaded(int /*pageId*/, int frameIndex) override;

private:
    std::queue<int> frameQueue; ///< Queue of frames in loading order.
};

#endif // CORE_ALGORITHMS_FIFOALGORITHM_H
