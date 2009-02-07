#ifndef ARMY_TEST_ELF_LOADER_H_
#define ARMY_TEST_ELF_LOADER_H_

#include <cppunit/extensions/HelperMacros.h>

#include "loader/ELFLoader.h"

class ELFLoaderTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(ELFLoaderTest);
    CPPUNIT_TEST(testGarbage);
    CPPUNIT_TEST(testLoad);
    CPPUNIT_TEST_SUITE_END();

public:
    void testGarbage();
    void testLoad();
};

#endif
