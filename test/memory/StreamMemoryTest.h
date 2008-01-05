#ifndef STREAM_MEMORY_TEST_H
#define STREAM_MEMORY_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "test/memory/MemoryTest.h"

class StreamMemoryTest : public MemoryTest {
    CPPUNIT_TEST_SUB_SUITE(StreamMemoryTest, MemoryTest);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void testRead();
};

#endif
