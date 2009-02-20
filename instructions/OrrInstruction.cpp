#include "instructions/OrrInstruction.h"
#include "instructions/RegArg.h"

namespace arm {

OrrInstruction::OrrInstruction(CPURegisters::Register dest, Arg *op1, Arg *sh_op2, bool update_cpsr, Cond cond)
:
DPInstruction(cond, dest, op1, sh_op2, update_cpsr, true, true, false, false, false)
{
}

ARM_Word OrrInstruction::compute_result(ARM_Word op1, ARM_Word op2, bool, bool, ProgramStatusRegister&) const
{
    return op1 | op2;
}

const char *OrrInstruction::mnemonic() const
{
    return "ORR";
}

namespace is {

Instruction *ORR(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new OrrInstruction(dest, new RegArg(op1), sh_op2, S, cond);
}

}

}
