#include <cassert>
#include <map>

#include "test/core/CPURegistersTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CPURegistersTest);

static const ProgramStatusRegister::Mode modes[] = {
    ProgramStatusRegister::User,
    ProgramStatusRegister::System,
    ProgramStatusRegister::Abort,
    ProgramStatusRegister::Undefined,
    ProgramStatusRegister::Supervisor,
    ProgramStatusRegister::IRQ,
    ProgramStatusRegister::FIQ
};

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

static const int mode_count = sizeof(modes)/sizeof(modes[0]);
static const int bit_count = sizeof(bits)/sizeof(bits[0]);

void CPURegistersTest::setUp() {
    regs_ = new CPURegisters();

    //we always start in User mode
    ProgramStatusRegister cpsr = regs_->get_status_reg(CPURegisters::CPSR);
    cpsr.set_mode(ProgramStatusRegister::User);
    regs_->set_status_reg(CPURegisters::CPSR, cpsr);
}

void CPURegistersTest::tearDown() {
    delete regs_;
}

void CPURegistersTest::testRegs() {


    for (int i = 0; i < mode_count; i++) {
        ProgramStatusRegister::Mode mode = modes[i];
        ProgramStatusRegister cpsr = regs_->get_status_reg(CPURegisters::CPSR);
        cpsr.set_mode(mode);
        regs_->set_status_reg(CPURegisters::CPSR, cpsr);

    //Write all registers a unique value
        regs_->set_reg(CPURegisters::R0,  1 + 16*i);
        regs_->set_reg(CPURegisters::R1,  2 + 16*i);
        regs_->set_reg(CPURegisters::R2,  3 + 16*i);
        regs_->set_reg(CPURegisters::R3,  4 + 16*i);
        regs_->set_reg(CPURegisters::R4,  5 + 16*i);
        regs_->set_reg(CPURegisters::R5,  6 + 16*i);
        regs_->set_reg(CPURegisters::R6,  7 + 16*i);
        regs_->set_reg(CPURegisters::R7,  8 + 16*i);
        regs_->set_reg(CPURegisters::R8,  9 + 16*i);
        regs_->set_reg(CPURegisters::R9,  10 + 16*i);
        regs_->set_reg(CPURegisters::R10, 11 + 16*i);
        regs_->set_reg(CPURegisters::R11, 12 + 16*i);
        regs_->set_reg(CPURegisters::R12, 13 + 16*i);
        regs_->set_reg(CPURegisters::R13, 14 + 16*i);
        regs_->set_reg(CPURegisters::R14, 15 + 16*i);
        regs_->set_reg(CPURegisters::R15, 16 + 16*i);

#define MY_TEST(reg,b) CPPUNIT_ASSERT_EQUAL((ARM_Word)(reg + 1 + 16*(i-b)), regs_->get_reg(CPURegisters::R##reg))
#define MY_TEST_SAME(reg) MY_TEST(reg,1)
#define MY_TEST_OVER(reg) MY_TEST(reg,0)

        //Now read it back and check it is the same
        MY_TEST_OVER(0 );
        MY_TEST_OVER(1 );
        MY_TEST_OVER(2 );
        MY_TEST_OVER(3 );
        MY_TEST_OVER(4 );
        MY_TEST_OVER(5 );
        MY_TEST_OVER(6 );
        MY_TEST_OVER(7 );
        MY_TEST_OVER(8 );
        MY_TEST_OVER(9 );
        MY_TEST_OVER(10);
        MY_TEST_OVER(11);
        MY_TEST_OVER(12);
        MY_TEST_OVER(13);
        MY_TEST_OVER(14);
        MY_TEST_OVER(15);

        //Check that CPSR and SPSR did not get overwritten
        //the "<<" operator for ostream would have to be written for
        //ProgramStatusRegister for this test to be "doable"
//        CPPUNIT_ASSERT_EQUAL_MESSAGE("CPSR and SPSR must not be written on base register writing",
//            cpsr, regs_->get_status_reg(CPURegisters::CPSR));

        //Now check that the values in previous modes did not overwrite
        //anything where banked registers are supposed prevent overwriting
        //and check that all values registers that are not banked _did_ get
        //overwritten
        ProgramStatusRegister::Mode prev_mode;
        if (mode != ProgramStatusRegister::User) {
            cpsr = regs_->get_status_reg(CPURegisters::CPSR);
            prev_mode = modes[i-1];
            cpsr.set_mode(prev_mode);
            regs_->set_status_reg(CPURegisters::CPSR, cpsr);

            switch (mode) {
            case ProgramStatusRegister::FIQ:
                MY_TEST_OVER(15);

                MY_TEST_SAME(14);
                MY_TEST_SAME(13);
                MY_TEST_SAME(12);
                MY_TEST_SAME(11);
                MY_TEST_SAME(10);
                MY_TEST_SAME(9 );
                MY_TEST_SAME(8 );

                MY_TEST_OVER(7 );
                MY_TEST_OVER(6 );
                MY_TEST_OVER(5 );
                MY_TEST_OVER(4 );
                MY_TEST_OVER(3 );
                MY_TEST_OVER(2 );
                MY_TEST_OVER(1 );
                MY_TEST_OVER(0 );
                break;
            case ProgramStatusRegister::Abort:
            case ProgramStatusRegister::IRQ:
            case ProgramStatusRegister::Supervisor:
            case ProgramStatusRegister::Undefined:
                MY_TEST_OVER(15);

                MY_TEST_SAME(14);
                MY_TEST_SAME(13);
                
                MY_TEST_OVER(12);
                MY_TEST_OVER(11);
                MY_TEST_OVER(10);
                MY_TEST_OVER(9 );
                MY_TEST_OVER(8 );
                MY_TEST_OVER(7 );
                MY_TEST_OVER(6 );
                MY_TEST_OVER(5 );
                MY_TEST_OVER(4 );
                MY_TEST_OVER(3 );
                MY_TEST_OVER(2 );
                MY_TEST_OVER(1 );
                MY_TEST_OVER(0 );
                break;
            case ProgramStatusRegister::System:
                MY_TEST_OVER(15);
                MY_TEST_OVER(14);
                MY_TEST_OVER(13);
                MY_TEST_OVER(12);
                MY_TEST_OVER(11);
                MY_TEST_OVER(10);
                MY_TEST_OVER(9 );
                MY_TEST_OVER(8 );
                MY_TEST_OVER(7 );
                MY_TEST_OVER(6 );
                MY_TEST_OVER(5 );
                MY_TEST_OVER(4 );
                MY_TEST_OVER(3 );
                MY_TEST_OVER(2 );
                MY_TEST_OVER(1 );
                MY_TEST_OVER(0 );
                break;

            case ProgramStatusRegister::User:
                assert(!"User mode is the first mode tested - no previously tested mode is available.");
                break;
            default:
                assert(!"Unknown mode");
                break;
            }
        }
    }
}

static bool set_bit(int mode, int bit)
{
    return (((mode - bit) % 2) == 0);
}

static void set_mode(CPURegisters &regs, ProgramStatusRegister::Mode mode)
{
    ProgramStatusRegister cpsr = regs.get_status_reg(CPURegisters::CPSR);
    cpsr.set_mode(mode);
    regs.set_status_reg(CPURegisters::CPSR, cpsr);
}

static void compare_psr_bits(const ProgramStatusRegister &expected, const ProgramStatusRegister &actual)
{
    for (int bit_ = 0; bit_ < bit_count; bit_++) {
        ProgramStatusRegister::Bit bit = bits[bit_];
        CPPUNIT_ASSERT_EQUAL(expected.get_bit(bit), actual.get_bit(bit));
    }
}

/**
 * Tests that CPSR does not change in different modes (except for the mode itself)
 */
void CPURegistersTest::testCPSR() {
    //for each mode, set CPSR bits to per-mode-unique values and
    //check that the bits propagated to all modes
    for (int mode_ = 0; mode_ < mode_count; mode_++) {
        ProgramStatusRegister::Mode mode = modes[mode_];
        set_mode(*regs_, mode);
        //prepare a per-mode unique CPSR-bits value
        ProgramStatusRegister psr = regs_->get_status_reg(CPURegisters::CPSR);
        for (int bit_ = 0; bit_ < bit_count; bit_++) {
            ProgramStatusRegister::Bit bit = bits[bit_];
            psr.set_bit(bit, set_bit(mode_, bit_));
        }
        //set the value
        regs_->set_status_reg(CPURegisters::CPSR, psr);
        //see that in every mode, the value of bits is as in psr
        for (int mode2_ = 0; mode2_ < mode_count; mode2_++) {
            //change mode
            ProgramStatusRegister::Mode mode2 = modes[mode2_];
            set_mode(*regs_, mode2);
            //get the mode's CPSR
            ProgramStatusRegister actual_cpsr = regs_->get_status_reg(CPURegisters::CPSR);
            compare_psr_bits(psr, actual_cpsr);
        }
    }
}

/**
 * Test that User and System mode do not have SPSR and that access to them
 * in these modes leads to RuntimeException.
 * Further, test that each of the remaining modes has a separate SPSR.
 */
typedef std::map<ProgramStatusRegister::Mode, ProgramStatusRegister> PSRMap;
void CPURegistersTest::testSPSR() {
    //prepare testing values
    PSRMap spsr_regs;
    for (int mode_ = 0; mode_ < mode_count; mode_++) {
        ProgramStatusRegister::Mode mode = modes[mode_];
        if (mode == ProgramStatusRegister::User || mode == ProgramStatusRegister::System)
            //these modes do not have SPSR
            continue;

        //to enhance test, don't save current mode's mode in SPSR, but a different
        //value - might detect that changing CPSR affects SPSR (which it should not)
        const int spsr_mode_shift = 2;
        ProgramStatusRegister::Mode saved_mode = modes[(mode_ + spsr_mode_shift) % mode_count];
        ProgramStatusRegister saved_spsr;
        saved_spsr.set_mode(saved_mode);
        for (int bit_ = 0; bit_ < bit_count; bit_++) {
            ProgramStatusRegister::Bit bit = bits[bit_];
            saved_spsr.set_bit(bit, set_bit(mode_ + spsr_mode_shift, bit_));
        }
        spsr_regs[mode] = saved_spsr;
        //and now save the value to the appropriate mode's SPSR
        set_mode(*regs_, mode);
        CPPUNIT_ASSERT_NO_THROW(regs_->set_status_reg(CPURegisters::SPSR, saved_spsr));
        CPPUNIT_ASSERT_NO_THROW(regs_->get_status_reg(CPURegisters::SPSR));
    }

    ProgramStatusRegister cpsr = regs_->get_status_reg(CPURegisters::CPSR);
    cpsr.set_bit(ProgramStatusRegister::N);
    cpsr.set_bit(ProgramStatusRegister::F);
    regs_->set_status_reg(CPURegisters::CPSR, cpsr);
    //for each mode
    for (int mode_ = 0; mode_ < mode_count; mode_++) {
        ProgramStatusRegister::Mode mode = modes[mode_];
        set_mode(*regs_, mode);

        if (mode == ProgramStatusRegister::User || mode == ProgramStatusRegister::System) {
            CPPUNIT_ASSERT_THROW(regs_->set_status_reg(CPURegisters::SPSR, ProgramStatusRegister()), RuntimeException);
            CPPUNIT_ASSERT_THROW(regs_->get_status_reg(CPURegisters::SPSR), RuntimeException);
            continue;
        }

        ProgramStatusRegister spsr_saved = spsr_regs[mode];
        regs_->set_status_reg(CPURegisters::SPSR, spsr_saved);
        ProgramStatusRegister cpsr_actual = regs_->get_status_reg(CPURegisters::CPSR);
        compare_psr_bits(cpsr, cpsr_actual);

        //check that set_status_reg above did not overwrite other mode's SPSR
        for (int mode2_ = 0; mode2_ < mode_count; mode2_++) {
            ProgramStatusRegister::Mode mode2 = modes[mode2_];
            set_mode(*regs_, mode2);
            cpsr.set_mode(mode2);
            CPPUNIT_ASSERT_EQUAL(cpsr, regs_->get_status_reg(CPURegisters::CPSR));

            if (mode2 == ProgramStatusRegister::User || mode2 == ProgramStatusRegister::System) {
                CPPUNIT_ASSERT_THROW(regs_->set_status_reg(CPURegisters::SPSR, ProgramStatusRegister()), RuntimeException);
                CPPUNIT_ASSERT_THROW(regs_->get_status_reg(CPURegisters::SPSR), RuntimeException);
                continue;
            }

            ProgramStatusRegister spsr_actual = regs_->get_status_reg(CPURegisters::SPSR);
            ProgramStatusRegister spsr_saved2 = spsr_regs[mode2];
            CPPUNIT_ASSERT_EQUAL(spsr_saved2, spsr_actual);
        }
    }
}
