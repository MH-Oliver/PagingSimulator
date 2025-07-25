#ifndef DUMMYPAGINGALGORITHM_H
#define DUMMYPAGINGALGORITHM_H
#include "PagingAlgorithm.h"


class DummyPagingAlgorithm : public PagingAlgorithm {
public:
    void memoryAccess(int pageId) {

    }

    int selectVictimPage() override {
        static int count = 0;
        if (count >= 2) count = 0;
        return count++;
    }

    void pageLoaded(int pageId, int frameIndex) override {

    }
};



#endif //DUMMYPAGINGALGORITHM_H
