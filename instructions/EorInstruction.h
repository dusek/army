#ifndef ARMY_CORE_EORINSTRUCTION_H_
#define ARMY_CORE_EORINSTRUCTION_H_

#include "instructions/DPInstruction.h"

namespace arm {
    class EorInstruction : public DPInstruction
    {
    public:
        EorInstruction(Cond cond, CPURegisters::Register dest, Arg *op1, Arg *op2, bool update_cpsr, bool store_result);

        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;
    };

    namespace is {
        Instruction *EOR(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S = false, Cond cond = AL);
        Instruction *TEQ(CPURegisters::Register op1, Arg *sh_op2, Cond cond = AL);
    }
}

#endif
