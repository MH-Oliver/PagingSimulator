/**
* @file LRUAlgorithm.h
 * @brief Least-Recently-Used page replacement.
 */
#ifndef CORE_ALGORITHMS_LRUALGORITHM_H
#define CORE_ALGORITHMS_LRUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>

/**
 * @brief LRU replacement based on a logical access counter.
 */
class LRUAlgorithm : public PagingAlgorithm {
public:
    LRUAlgorithm();
    ~LRUAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    int  selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    long accessCounter; ///< Monotonic counter incremented on each access.
    struct Info { int frameIndex; long lastUse; };
    std::unordered_map<int, Info> table; ///< pageId -> Info
};

#endif // CORE_ALGORITHMS_LRUALGORITHM_H
