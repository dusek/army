#include "test/core/ProgramStatusRegisterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ProgramStatusRegisterTest);

void ProgramStatusRegisterTest::setUp()
{
    psr_ = new ProgramStatusRegister();
}

void ProgramStatusRegisterTest::tearDown()
{
    delete psr_;
}

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

    for (int i = 0; i < mode_count; i++) {
        ProgramStatusRegister::Mode mode = modes[i];
        psr_->set_mode(mode);
        CPPUNIT_ASSERT_EQUAL(mode, psr_->get_mode());
    }
    //TODO test that the bits like N,Z etc. did not change
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

        psr_->set_bit(bit);
        CPPUNIT_ASSERT_EQUAL(true, psr_->get_bit(bit));

        psr_->set_bit(bit, false);
        CPPUNIT_ASSERT_EQUAL(false, psr_->get_bit(bit));
    }
}