/**
* @file SecondChanceAlgorithm.h
 * @brief Second-Chance (Clock) page replacement algorithm.
 */
#ifndef CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H
#define CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <list>
#include <unordered_map>
#include <stdexcept>

/**
 * @brief Second-Chance using a circular list and referenced bits.
 */
class SecondChanceAlgorithm : public PagingAlgorithm {
public:
    SecondChanceAlgorithm() = default;
    ~SecondChanceAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    int  selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    struct Entry { int pageId; int frameIndex; bool referenced; };
    std::list<Entry> clockList; ///< Clock list (front = hand).
    std::unordered_map<int, std::list<Entry>::iterator> pageMap; ///< pageId -> node.
};

#endif // CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H
