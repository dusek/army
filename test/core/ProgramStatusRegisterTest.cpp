#include "test/core/ProgramStatusRegisterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ProgramStatusRegisterTest);

static const ProgramStatusRegister::Bit bits[] = {
    ProgramStatusRegister::N,
    ProgramStatusRegister::Z,
    ProgramStatusRegister::C,
    ProgramStatusRegister::V,
    ProgramStatusRegister::Q,
    ProgramStatusRegister::F,
    ProgramStatusRegister::I,
    ProgramStatusRegister::T
};

static const int bit_count = sizeof(bits)/sizeof(bits[0]);

void ProgramStatusRegisterTest::setUp()
{
    psr_ = new ProgramStatusRegister();
}

void ProgramStatusRegisterTest::tearDown()
{
    delete psr_;
}

/**
 * Tests set-get of mode
 */
void ProgramStatusRegisterTest::testMode()
{
    static const ProgramStatusRegister::Mode modes[] = {
        ProgramStatusRegister::User,
        ProgramStatusRegister::System,
        ProgramStatusRegister::Abort,
        ProgramStatusRegister::Undefined,
        ProgramStatusRegister::Supervisor,
        ProgramStatusRegister::FIQ,
        ProgramStatusRegister::IRQ
    };

    const int mode_count = sizeof(modes)/sizeof(modes[0]);

    //for each mode, set the mode and then get it and compare to the set mode
    for (int i = 0; i < mode_count; i++) {
        ProgramStatusRegister::Mode mode = modes[i];
        psr_->set_mode(mode);
        CPPUNIT_ASSERT_EQUAL(mode, psr_->get_mode());
        //also test that setting mode did not change other bits
        for (int bit_ = 0; bit_ < bit_count; bit_++) {
            //FIXME this assumes that default constructor sets all bits to 0
            //(and that setUp() uses default constructor and nothing else)
            //it's true at this moment, but might change in the future
            //(e.g. if I find non-zero default values in ARM documentation)
            CPPUNIT_ASSERT_EQUAL(psr_->get_bit(bits[bit_]), false);
        }
    }
}

void ProgramStatusRegisterTest::testBits()
{
    static const ProgramStatusRegister::Bit bits[] = {
        ProgramStatusRegister::N,
        ProgramStatusRegister::Z,
        ProgramStatusRegister::C,
        ProgramStatusRegister::V,
        ProgramStatusRegister::F,
        ProgramStatusRegister::Q,
        ProgramStatusRegister::I
    };
    const int bit_count = sizeof(bits)/sizeof(bits[0]);

    for (int i = 0; i < bit_count; i++) {
        ProgramStatusRegister::Bit bit = bits[i];
        ProgramStatusRegister::Mode mode = psr_->get_mode();

        psr_->set_bit(bit);
        CPPUNIT_ASSERT_EQUAL(true, psr_->get_bit(bit));
        CPPUNIT_ASSERT_EQUAL(mode, psr_->get_mode());

        psr_->set_bit(bit, false);
        CPPUNIT_ASSERT_EQUAL(false, psr_->get_bit(bit));
        CPPUNIT_ASSERT_EQUAL(mode, psr_->get_mode());
    }
}
