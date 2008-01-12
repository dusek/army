#ifndef ARMY_CORE_CPU_STATE_H
#define ARMY_CORE_CPU_STATE_H

#include "core/ProgramStatusRegister.h"
#include "core/RuntimeException.h"

#include "armyconfig.h"

class ARMYCORE_EXPORT CPURegisters {
public:
    CPURegisters();

    typedef enum {
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        SP = R13,
        LR = R14,
        PC = R15
    } Register;

    typedef enum {
        CPSR,
        SPSR
    } StatusRegister;

    //TODO would it not be better to have just "ARM_Word &reg(Register reg)" ?
    //maybe not really for performance reasons (reference on very small type)
    ARM_Word get_reg(Register reg) const;
    void     set_reg(Register reg, ARM_Word value);

    ProgramStatusRegister get_status_reg(StatusRegister reg) const;
    void                  set_status_reg(StatusRegister reg, const ProgramStatusRegister &value);
    
private:
    class CPURegistersImpl;
    CPURegistersImpl *pimpl;
};

#endif

