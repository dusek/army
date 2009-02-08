#ifndef ARMY_TEST_VIRTUAL_MEMORY_TEST_H
#define ARMY_TEST_VIRTUAL_MEMORY_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "test/memory/MemoryTest.h"

class VirtualMemoryTest : public MemoryTest {
public:
    CPPUNIT_TEST_SUB_SUITE(VirtualMemoryTest, MemoryTest);
    CPPUNIT_TEST(testAll);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();    
    void testAll();
};

#endif
