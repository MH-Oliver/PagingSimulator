/**
* @file NFUAlgorithm.h
 * @brief Not-Frequently-Used with simple aging (right shift on selection).
 */
#ifndef CORE_ALGORITHMS_NFUALGORITHM_H
#define CORE_ALGORITHMS_NFUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <limits>
#include <stdexcept>

/**
 * @brief NFU: evict the page with the smallest (aged) reference count.
 */
class NFUAlgorithm : public PagingAlgorithm {
public:
    NFUAlgorithm() = default;
    ~NFUAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    int  selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    struct Info { int frameIndex; unsigned int counter; };
    std::unordered_map<int, Info> table; ///< pageId -> Info
};

#endif // CORE_ALGORITHMS_NFUALGORITHM_H
