/**
* @file NFUAlgorithm.h
 * @brief Not-Frequently-Used with classical aging: right shift and R-bit injection into MSB.
 */
#ifndef CORE_ALGORITHMS_NFUALGORITHM_H
#define CORE_ALGORITHMS_NFUALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <limits>

/**
 * @brief NFU with classical aging.
 * @details Each resident page keeps an 8-bit 'age' and a one-bit 'referenced' flag.
 *          On each replacement decision (or periodically if extended), all ages are updated:
 *              age = (age >> 1) | (referenced ? 0x80 : 0x00);
 *              referenced = false;
 *          Victim is the page with the smallest age (coldest).
 */
class NFUAlgorithm : public PagingAlgorithm {
public:
  NFUAlgorithm() = default;
  ~NFUAlgorithm() override = default;

  /** @brief Mark page as referenced for this access. */
  void memoryAccess(int pageId) override;

  /**
   * @brief Update all ages via right shift + MSB injection of R, then pick the coldest page.
   * @return Victim frame index.
   * @throws std::logic_error if table is empty.
   */
  int  selectVictimPage() override;

  /** @brief Register a freshly loaded page with age=0 and referenced=true (first access). */
  void pageLoaded(int pageId, int frameIndex) override;

private:
  struct Info {
    int      frameIndex;
    uint8_t  age;         ///< 8-bit aging counter (higher = more recently used).
    bool     referenced;  ///< R-flag collected since last aging.
  };
  std::unordered_map<int, Info> table; ///< pageId -> Info
};

#endif // CORE_ALGORITHMS_NFUALGORITHM_H
