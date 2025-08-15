// src/core/DummyPagingAlgorithm.h
/**
 * @file DummyPagingAlgorithm.h
 * @brief Minimal paging algorithm for quick testing.
 */
#ifndef DUMMYPAGINGALGORITHM_H
#define DUMMYPAGINGALGORITHM_H

#include "core/PagingAlgorithm.h"

/**
 * @brief A trivial algorithm that cycles over a small set of frames.
 */
class DummyPagingAlgorithm : public PagingAlgorithm {
public:
    void memoryAccess(int /*pageId*/) override {}
    int  selectVictimPage() override {
        static int count = 0;
        if (count >= 2) count = 0;
        return count++;
    }
    void pageLoaded(int /*pageId*/, int /*frameIndex*/) override {}
};

#endif // DUMMYPAGINGALGORITHM_H
