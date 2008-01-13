#include <cassert>

#include "test/core/CPURegistersTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CPURegistersTest);

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
    const ProgramStatusRegister::Mode modes[] = {
        ProgramStatusRegister::User,
        ProgramStatusRegister::System,
        ProgramStatusRegister::Abort,
        ProgramStatusRegister::Undefined,
        ProgramStatusRegister::Supervisor,
        ProgramStatusRegister::IRQ,
        ProgramStatusRegister::FIQ
    };

    const int mode_count = sizeof(modes)/sizeof(modes[0]);

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
