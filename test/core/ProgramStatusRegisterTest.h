#ifndef ARMY_CORE_TEST_PROGRAM_STATUS_REGISTER_TEST_H_
#define ARMY_CORE_TEST_PROGRAM_STATUS_REGISTER_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "core/ProgramStatusRegister.h"

class ProgramStatusRegisterTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(ProgramStatusRegisterTest);
    CPPUNIT_TEST(testMode);
    CPPUNIT_TEST(testBits);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testMode();
    void testBits();
private:
    ProgramStatusRegister *psr_;
};

#endif
