//
// Created by Son on 28.07.25.
//

#ifndef CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H
#define CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <list>
#include <unordered_map>

class SecondChanceAlgorithm : public PagingAlgorithm {
public:
    SecondChanceAlgorithm();
    ~SecondChanceAlgorithm() override = default;
    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;
private:
    struct Entry { int pageId; int frameIndex; bool referenced; };
    std::list<Entry> clockList;
    std::unordered_map<int, std::list<Entry>::iterator> pageMap;
};

#endif // CORE_ALGORITHMS_SECONDCHANCEALGORITHM_H