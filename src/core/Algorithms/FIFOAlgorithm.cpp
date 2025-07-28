//
// Created by Son on 28.07.25.
//

#include "core/algorithms/FIFOAlgorithm.h"

FIFOAlgorithm::FIFOAlgorithm() = default;

void FIFOAlgorithm::memoryAccess(int /*pageId*/) {
    // FIFO does not track accesses
}

int FIFOAlgorithm::selectVictimPage() {
    int victim = frameQueue.front();
    frameQueue.pop();
    return victim;
}

void FIFOAlgorithm::pageLoaded(int /*pageId*/, int frameIndex) {
    frameQueue.push(frameIndex);
}