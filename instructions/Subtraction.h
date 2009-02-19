#ifndef ARMY_CORE_SUB_H_
#define ARMY_CORE_SUB_H_

#include "instructions/DPInstruction.h"

namespace arm {

    class Subtraction : public DPInstruction
    {
    public:
        Subtraction(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool reverse_args, bool has_carry);

    protected:
        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;
    };

    namespace is {
        Instruction *SUB(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S = false, Cond cond = AL);
        Instruction *SBC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S = false, Cond cond = AL);
        Instruction *RSB(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S = false, Cond cond = AL);
        Instruction *RSC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S = false, Cond cond = AL);
        Instruction *CMP(CPURegisters::Register op1, Arg *sh_op, Cond cond = AL);
    }
}

#endif
