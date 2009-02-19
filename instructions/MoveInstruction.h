#ifndef ARMY_CORE_MOVEINSTRUCTION_H_
#define ARMY_CORE_MOVEINSTRUCTION_H_

#include "instructions/DPInstruction.h"

namespace arm {

    class MoveInstruction : public DPInstruction
    {
    public:
        MoveInstruction(CPURegisters::Register dest, Arg *sh_op, bool update_cpsr, bool not_, Cond cond);

        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const;
        virtual const char *mnemonic() const;

    private:
        bool not_;
    };

    namespace is {
        Instruction *MOV(CPURegisters::Register dest, Arg *sh_op, bool S = false, Cond cond = AL);
        Instruction *MVN(CPURegisters::Register dest, Arg *sh_op, bool S = false, Cond cond = AL);
    }
}

#endif
