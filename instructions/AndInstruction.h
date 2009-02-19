#ifndef ARMY_CORE_ANDINSTRUCTION_H_
#define ARMY_CORE_ANDINSTRUCTION_H_

#include "instructions/DPInstruction.h"

namespace arm {
    class AndInstruction : public DPInstruction
    {
    public:
        AndInstruction(Cond cond, CPURegisters::Register dest, Arg *op1, Arg *op2, bool update_cpsr, bool store_result, bool op2_not);

        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;

    private:
        bool op2_not_;
    };

    namespace is {
        Instruction *AND(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S = false, Cond cond = AL);
        Instruction *TST(CPURegisters::Register op1, Arg *sh_op2, Cond cond = AL);
        Instruction *BIC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S = false, Cond cond = AL);
    }
}

#endif
