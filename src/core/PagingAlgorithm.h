/**
* @file PagingAlgorithm.h
 * @brief Abstract base interface for paging replacement algorithms.
 */
#ifndef PAGINGALGORITHM_H
#define PAGINGALGORITHM_H

class PagingAlgorithm {
public:
 virtual ~PagingAlgorithm() = default;

 /**
  * @brief Notify the algorithm about an access to a page.
  * @param pageId Virtual page ID that was accessed.
  */
 virtual void memoryAccess(int pageId) = 0;

 /**
  * @brief Select the victim frame index when memory is full.
  * @return Frame index to evict.
  */
 virtual int selectVictimPage() = 0;

 /**
  * @brief Notify that a page has been loaded into a specific frame.
  * @param pageId Virtual page ID.
  * @param frameIndex Physical frame index.
  */
 virtual void pageLoaded(int pageId, int frameIndex) = 0;

 /**
  * @brief Optional hook for write accesses (dirty tracking).
  * @param pageId Virtual page ID that was written.
  */
 virtual void onWrite(int /*pageId*/) {}
};

#endif // PAGINGALGORITHM_H
