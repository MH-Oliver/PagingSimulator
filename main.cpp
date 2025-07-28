#include "src/core/DummyPagingAlgorithm.h"
#include "test/DesTest.h"
#include "test/HandleMemoryAccessTest.h"
#include "core/algorithms/FIFOAlgorithm.h"
#include "core/algorithms/SecondChanceAlgorithm.h"
#include "core/algorithms/LRUAlgorithm.h"
#include "core/algorithms/NRUAlgorithm.h"
#include "core/algorithms/NFUAlgorithm.h"

int main() {
    DesTest::executeTests();
    //HandleMemoryAccessTest::executeTests(new FIFOAlgorithm());
    //HandleMemoryAccessTest::executeTests(new SecondChanceAlgorithm());
    //HandleMemoryAccessTest::executeTests(new LRUAlgorithm());
    //HandleMemoryAccessTest::executeTests(new NRUAlgorithm());
    //HandleMemoryAccessTest::executeTests(new NFUAlgorithm());
}