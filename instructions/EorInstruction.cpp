#include "instructions/EorInstruction.h"
#include "instructions/RegArg.h"

namespace arm {

EorInstruction::EorInstruction(Cond cond, CPURegisters::Register dest, Arg *op1, Arg *op2, bool update_cpsr, bool store_result)
:
DPInstruction(cond, dest, op1, op2, update_cpsr, store_result, true, false, false, false)
{
}

ARM_Word EorInstruction::compute_result(ARM_Word op1, ARM_Word op2, bool, bool, ProgramStatusRegister&) const
{
    ARM_Word result = (op1 & ~op2) | (~op1 & op2);
    return result;
}

const char *EorInstruction::mnemonic() const
{
    if (store_result_)
        return "EOR";
    else
        return "TEQ";
}

namespace is {

Instruction *EOR(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new EorInstruction(cond, dest, new RegArg(op1), sh_op2, S, true);
}

Instruction *TEQ(CPURegisters::Register op1, Arg *sh_op2, Cond cond)
{
    return new EorInstruction(cond, CPURegisters::R0, new RegArg(op1), sh_op2, true, false);
}

}

}
