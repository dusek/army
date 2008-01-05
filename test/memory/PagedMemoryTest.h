#ifndef PAGED_MEMORY_TEST_H
#define PAGED_MEMORY_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "test/memory/MemoryTest.h"

class PagedMemoryTest : public MemoryTest {
public:
    CPPUNIT_TEST_SUB_SUITE(PagedMemoryTest, MemoryTest);
    CPPUNIT_TEST(testAll);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();    
    void testAll();
};

#endif
