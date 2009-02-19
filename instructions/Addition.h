#ifndef ARMY_CORE_ADDITION_H_
#define ARMY_CORE_ADDITION_H_

#include "instructions/DPInstruction.h"

namespace arm {

    class Addition : public DPInstruction
    {
    public:
        Addition(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool has_carry);

        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;
    };

    namespace is {
        Instruction *ADD(CPURegisters::Register dest_reg, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond = AL);
        Instruction *ADC(CPURegisters::Register dest_reg, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond = AL);
        Instruction *CMN(CPURegisters::Register op1, Arg *sh_op2, Cond cond = AL);
    }

}

#endif
