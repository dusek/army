#ifndef ARMY_CORE_ORRINSTRUCTION_H_
#define ARMY_CORE_ORRINSTRUCTION_H_

#include "instructions/DPInstruction.h"

namespace arm {

    class OrrInstruction : public DPInstruction
    {
    public:
        OrrInstruction(CPURegisters::Register dest, Arg *op1, Arg *sh_op2, bool update_cpsr, Cond cond);

        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;
    };

    namespace is {
        Instruction *ORR(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S = false, Cond cond = AL);
    }
}

#endif
