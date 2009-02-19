#include "instructions/MoveInstruction.h"

namespace arm {

MoveInstruction::MoveInstruction(CPURegisters::Register dest, Arg *sh_op, bool update_cpsr, bool not_, Cond cond)
:
DPInstruction(cond, dest, 0, sh_op, update_cpsr, true, true, false, false, false),
not_(not_)
{
}

ARM_Word MoveInstruction::compute_result(ARM_Word, ARM_Word op2, bool, bool, ProgramStatusRegister &) const
{
    if (not_)
        op2 = ~op2;
    return op2;
}

const char *MoveInstruction::mnemonic() const
{
    if (not_)
        return "MVN";
    else
        return "MOV";
}

namespace is {

Instruction *MOV(CPURegisters::Register dest, Arg *sh_op, bool S, Cond cond)
{
    return new MoveInstruction(dest, sh_op, S, false, cond);
}

Instruction *MVN(CPURegisters::Register dest, Arg *sh_op, bool S, Cond cond)
{
    return new MoveInstruction(dest, sh_op, S, true, cond);
}

}

}