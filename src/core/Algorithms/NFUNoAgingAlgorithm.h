/**
* @file NFUNoAgingAlgorithm.h
 * @brief Not-Frequently-Used without aging (pure counters).
 */
#ifndef CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H
#define CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <limits>
#include <stdexcept>

/**
 * @brief NFU w/o aging: evict the page with the smallest raw reference count.
 */
class NFUNoAgingAlgorithm : public PagingAlgorithm {
public:
    NFUNoAgingAlgorithm() = default;
    ~NFUNoAgingAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    int  selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    struct Info { int frameIndex; unsigned int counter; };
    std::unordered_map<int, Info> table;
};

#endif // CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H
