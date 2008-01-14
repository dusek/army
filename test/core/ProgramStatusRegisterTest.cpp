#include <cassert>

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

    for (int k = 0; k < 3; k++) {
        switch(k) {
            case 0:
                for (int bit_ = 0; bit_ < bit_count; bit_++) {
                    psr_->set_bit(bits[bit_], true);
                }
                break;
            case 1:
                for (int bit_ = 0; bit_ < bit_count; bit_++) {
                    psr_->set_bit(bits[bit_], false);
                }
                break;
            case 2:
                psr_->set_bit(ProgramStatusRegister::C, true);
                psr_->set_bit(ProgramStatusRegister::T, true);
                psr_->set_bit(ProgramStatusRegister::N, true);
                break;
            default:
                assert(!"Unhandled case");
                break;
        }
        //for each mode, set the mode and then get it and compare to the set mode
        for (int i = 0; i < mode_count; i++) {
            ProgramStatusRegister::Mode mode = modes[i];
            psr_->set_mode(mode);
            CPPUNIT_ASSERT_EQUAL(mode, psr_->get_mode());

            //also test that setting mode did not change other bits
            for (int bit_ = 0; bit_ < bit_count; bit_++) {
                ProgramStatusRegister::Bit bit = bits[bit_];
                bool expected = true;
                switch(k) {
                case 0:
                    expected = true;
                    break;
                case 1:
                    expected = false;
                    break;
                case 2:
                    if (bit == ProgramStatusRegister::C
                     || bit == ProgramStatusRegister::T
                     || bit == ProgramStatusRegister::N)
                    {
                        expected = true;
                    }
                    else
                    {
                        expected = false;
                    }
                    break;
                default:
                    assert(!"Unhandled case");
                    break;
                }
                CPPUNIT_ASSERT_EQUAL(expected, psr_->get_bit(bit));
            }
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

void ProgramStatusRegisterTest::testEqual() {
    ProgramStatusRegister r1;
    ProgramStatusRegister r2;
    //test default-constructed values are equal
    CPPUNIT_ASSERT_EQUAL(r1, r2);

    //test assignment operator with modified mode
    r1.set_mode(ProgramStatusRegister::System);
    r2 = r1;
    CPPUNIT_ASSERT_EQUAL(r1, r2);

    //test assignment operator with modified bits
    r1.set_bit(ProgramStatusRegister::N, true);
    r1.set_bit(ProgramStatusRegister::Z, false);
    r2 = r1;
    CPPUNIT_ASSERT_EQUAL(r1, r2);

    //test PSRs created directly (without assignment operator) with
    //same operations to be equal
    ProgramStatusRegister r3;
    ProgramStatusRegister r4;

    r3.set_mode(ProgramStatusRegister::Supervisor);
    r4.set_mode(ProgramStatusRegister::Supervisor);

    r3.set_bit(ProgramStatusRegister::I, true);
    r4.set_bit(ProgramStatusRegister::I, true);
    r3.set_bit(ProgramStatusRegister::N, false);
    r4.set_bit(ProgramStatusRegister::N, false);
    CPPUNIT_ASSERT_EQUAL(r3, r4);

    //save the r3 value for which r3 == r4
    ProgramStatusRegister r5 = r3;

    //test that modifying a bit results in non-equality
    r3.set_bit(ProgramStatusRegister::I, false);
    CPPUNIT_ASSERT(!(r3==r4));

    //restore r3 value
    r3 = r5;
    //test that modifying a mode results in non-equality
    r3.set_mode(ProgramStatusRegister::FIQ);
    CPPUNIT_ASSERT(!(r3==r4));

    //test that modifying both results in non-equality
    r3.set_bit(ProgramStatusRegister::I, false);
    CPPUNIT_ASSERT(!(r3==r4));
}