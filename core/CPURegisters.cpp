#include <cassert>

#include "core/CPURegisters.h"

class SPSRNotAvailableException: public RuntimeException {
public:
    ProgramStatusRegister::Mode mode;

protected:
    void output_details(std::ostream &o) const {
        o << "Attempt to access SPSR in ";
        switch(mode) {
            case ProgramStatusRegister::User:
                o << "User";
                break;
            case ProgramStatusRegister::System:
                o << "System";
                break;
            default:
                assert(!"Mode not User or System in SPSR access exception.");
                break;
        }
        o << " mode. Results of this are defined as UNPREDICTABLE." << std::endl;
    };
};


typedef enum {
    R0_usr,
    R1_usr,
    R2_usr,
    R3_usr,
    R4_usr,
    R5_usr,
    R6_usr,
    R7_usr,
    R8_usr,
    R9_usr,
    R10_usr,
    R11_usr,
    R12_usr,
    R13_usr,
    R14_usr,
    R15_usr,
    R13_svc,
    R14_svc,
    R13_abt,
    R14_abt,
    R13_und,
    R14_und,
    R13_irq,
    R14_irq,
    R8_fiq,
    R9_fiq,
    R10_fiq,
    R11_fiq,
    R12_fiq,
    R13_fiq,
    R14_fiq,
    R15_fiq
} RealRegister;
enum { RealRegisterCount = R15_fiq + 1};

typedef enum {
    SPSR_svc,
    SPSR_abt,
    SPSR_und,
    SPSR_irq,
    SPSR_fiq
} RealSavedStatusRegister;
enum { RealSavedStatusRegisterCount = SPSR_fiq + 1 };

#define REAL_REGS_0_TO_7(suffix)\
    R0_##suffix,\
    R1_##suffix,\
    R2_##suffix,\
    R3_##suffix,\
    R4_##suffix,\
    R5_##suffix,\
    R6_##suffix,\
    R7_##suffix

#define REAL_REGS_8_TO_12(suffix)\
    R8_##suffix,\
    R9_##suffix,\
    R10_##suffix,\
    R11_##suffix,\
    R12_##suffix

#define REAL_REGS_13_TO_14(suffix)\
    R13_##suffix,\
    R14_##suffix

#define REAL_REGS_15(suffix)\
    R15_##suffix

#define REAL_REGS_ALL(a,b,c,d)\
    REAL_REGS_0_TO_7(a),\
    REAL_REGS_8_TO_12(b),\
    REAL_REGS_13_TO_14(c),\
    REAL_REGS_15(d)

#define REAL_REGS_COMMON(a)\
    REAL_REGS_ALL(usr,usr,a,usr)

#define REAL_REGS_USER REAL_REGS_COMMON(usr)
#define REAL_REGS_SYSTEM REAL_REGS_USER
#define REAL_REGS_SUPERVISOR REAL_REGS_COMMON(svc)
#define REAL_REGS_ABORT      REAL_REGS_COMMON(abt)
#define REAL_REGS_UNDEFINED  REAL_REGS_COMMON(und)
#define REAL_REGS_IRQ REAL_REGS_COMMON(irq)
#define REAL_REGS_FIQ\
    REAL_REGS_0_TO_7(usr),\
    REAL_REGS_8_TO_12(fiq),\
    REAL_REGS_13_TO_14(fiq),\
    REAL_REGS_15(usr)

class CPURegisters::CPURegistersImpl {
public:
    ARM_Word regs[RealRegisterCount];
    ProgramStatusRegister saved_status_regs[RealSavedStatusRegisterCount];
    ProgramStatusRegister cpsr; // CPSR = Current Program Status Register

    RealRegister get_reg_index(CPURegisters::Register reg) {
        static const RealRegister reg_map[7][16] = {
            { REAL_REGS_USER       },
            { REAL_REGS_FIQ        },
            { REAL_REGS_IRQ        },
            { REAL_REGS_SUPERVISOR },
            { REAL_REGS_ABORT      },
            { REAL_REGS_UNDEFINED  },
            { REAL_REGS_SYSTEM     }
        };

        ProgramStatusRegister::Mode mode = cpsr.get_mode();
        return reg_map[mode][reg];
    }

    RealSavedStatusRegister get_saved_status_reg_index() {
        ProgramStatusRegister::Mode mode = cpsr.get_mode();
        if (mode == ProgramStatusRegister::User ||
            mode == ProgramStatusRegister::System)
        {
            SPSRNotAvailableException e;
            e.mode = mode;
            throw e;
        }

        RealSavedStatusRegister realSavedReg = SPSR_fiq;
        switch (mode) {
            case ProgramStatusRegister::Supervisor:
                realSavedReg = SPSR_svc; break;
            case ProgramStatusRegister::Abort:
                realSavedReg = SPSR_abt; break;
            case ProgramStatusRegister::Undefined:
                realSavedReg = SPSR_und; break;
            case ProgramStatusRegister::IRQ:
                realSavedReg = SPSR_irq; break;
            case ProgramStatusRegister::FIQ:
                realSavedReg = SPSR_fiq; break;
            default:
                assert(!"Unhandled mode");
                //to make C4715 happy
                break;
        }

        return realSavedReg;
    }
};

CPURegisters::CPURegisters()
    : pimpl(new CPURegisters::CPURegistersImpl)
{
}

ARM_Word CPURegisters::get_reg(CPURegisters::Register reg) const {
    RealRegister realReg = pimpl->get_reg_index(reg);
    return pimpl->regs[realReg];
}

void CPURegisters::set_reg(CPURegisters::Register reg, ARM_Word value) {
    RealRegister realReg = pimpl->get_reg_index(reg);
    pimpl->regs[realReg] = value;
}

ProgramStatusRegister CPURegisters::get_status_reg(CPURegisters::StatusRegister reg) const
{
    if (reg == CPURegisters::CPSR)
        return pimpl->cpsr;
    else {
        RealSavedStatusRegister realSavedReg = pimpl->get_saved_status_reg_index();
        return pimpl->saved_status_regs[realSavedReg];
    }
}

void CPURegisters::set_status_reg(CPURegisters::StatusRegister reg, const ProgramStatusRegister &value)
{
    if (reg == CPURegisters::CPSR)
        pimpl->cpsr = value;
    else {
        RealSavedStatusRegister realSavedReg = pimpl->get_saved_status_reg_index();
        pimpl->saved_status_regs[realSavedReg] = value;
    }
}
