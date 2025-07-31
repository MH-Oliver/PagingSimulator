//
// Created by Son on 31.07.25.
//

#ifndef CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H
#define CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H

#include "core/PagingAlgorithm.h"
#include <unordered_map>
#include <climits>

/**
 * Not Frequently Used, ohne Aging
 * Bei jedem Speicherzugriff wird der Zähler um 1 erhöht.
 * Bei Auswahl des Opfers wird die Seite mit dem kleinsten Zähler entfernt.
 */
class NFUNoAgingAlgorithm : public PagingAlgorithm {
public:
    NFUNoAgingAlgorithm() = default;
    ~NFUNoAgingAlgorithm() override = default;

    void memoryAccess(int pageId) override;
    int selectVictimPage() override;
    void pageLoaded(int pageId, int frameIndex) override;

private:
    struct Info { int frameIndex; unsigned int counter; };
    std::unordered_map<int, Info> table;
};

#endif // CORE_ALGORITHMS_NFUNOAGINGALGORITHM_H