#include "instructions/AndInstruction.h"
#include "instructions/RegArg.h"

namespace arm {

AndInstruction::AndInstruction(Cond cond, CPURegisters::Register dest, Arg *op1, Arg *op2, bool update_cpsr, bool store_result, bool op2_not)
:
DPInstruction(cond, dest, op1, op2, update_cpsr, store_result, true, false, false, false),
op2_not_(op2_not)
{
}

ARM_Word AndInstruction::compute_result(ARM_Word op1, ARM_Word op2, bool, bool, ProgramStatusRegister&) const
{
    if (op2_not_)
        op2 = ~op2;
    ARM_Word result = op1 & op2;
    return result;
}

const char *AndInstruction::mnemonic() const
{
    if (!store_result_)
        return "TST";
    else if (op2_not_)
        return "BIC";
    else
        return "AND";
}

namespace is {

Instruction *AND(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new AndInstruction(cond, dest, new RegArg(op1), sh_op2, S, true, false);
}

Instruction *TST(CPURegisters::Register op1, Arg *sh_op2, Cond cond)
{
    return new AndInstruction(cond, CPURegisters::R0, new RegArg(op1), sh_op2, true, false, false);
}

Instruction *BIC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new AndInstruction(cond, dest, new RegArg(op1), sh_op2, S, true, true);
}

}

}
