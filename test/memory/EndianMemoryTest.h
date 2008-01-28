#ifndef ARMY_TEST_ENDIAN_MEMORY_H_
#define ARMY_TEST_ENDIAN_MEMORY_H_

#include <cppunit/extensions/HelperMacros.h>

#include "test/memory/MemoryTest.h"

class EndianMemoryTest : public MemoryTest {
public:
    CPPUNIT_TEST_SUB_SUITE(EndianMemoryTest, MemoryTest);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testWrite);
    CPPUNIT_TEST_SUITE_END();

public:
    void testRead();
    void testWrite();
};

#endif
