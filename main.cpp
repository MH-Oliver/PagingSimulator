#include "test/DesTest.h"
#include "test/HandleMemoryAccessTest.h"


int main() {
    DesTest::executeTests();
    HandleMemoryAccessTest::executeTests();
}