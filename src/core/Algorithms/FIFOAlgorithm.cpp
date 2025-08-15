// src/core/algorithms/FIFOAlgorithm.cpp
/**
 * @file FIFOAlgorithm.cpp
 * @brief Implementation of FIFO page replacement.
 */
#include "core/algorithms/FIFOAlgorithm.h"
#include <stdexcept>

int FIFOAlgorithm::selectVictimPage() {
    if (frameQueue.empty()) {
        throw std::logic_error("FIFO: empty queue");
    }
    int victim = frameQueue.front();
    frameQueue.pop();
    return victim;
}

void FIFOAlgorithm::pageLoaded(int /*pageId*/, int frameIndex) {
    frameQueue.push(frameIndex);
}
