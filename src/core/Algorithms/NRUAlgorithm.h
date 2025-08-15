/**
* @file NRUAlgorithm.h
 * @brief Not-Recently-Used with 4 classes (R,D) and periodic R-reset.
 */
#ifndef CORE_ALGORITHMS_NRUALGORITHM_H
#define CORE_ALGORITHMS_NRUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <vector>
#include <random>
#include <stdexcept>

/**
 * @brief NRU replacement using (referenced, dirty) classes.
 * @details Victim is chosen randomly from the lowest non-empty class:
 *          (R=0,D=0) -> (0,1) -> (1,0) -> (1,1). R bits are periodically reset.
 */
class NRUAlgorithm : public PagingAlgorithm {
public:
    /**
     * @brief Construct with a deterministic RNG seed.
     * @param seed Seed for random victim selection within a class.
     */
    explicit NRUAlgorithm(uint32_t seed = 0xC0FFEE);

    ~NRUAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    void onWrite(int pageId) override;         ///< Track dirty bit on writes.
    int  selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    struct Info { int frameIndex; bool referenced; bool dirty; };
    std::unordered_map<int, Info> table; ///< pageId -> Info
    std::mt19937 rng;                    ///< RNG for random tie-breaking.
    int accessCount{0};                  ///< Counts accesses to schedule resets.
    int resetPeriod{64};                 ///< Reset R every N accesses.

    int pickRandom(const std::vector<int>& v);
};

#endif // CORE_ALGORITHMS_NRUALGORITHM_H
