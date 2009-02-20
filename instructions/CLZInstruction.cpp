#include <bitset>

#include "instructions/CLZInstruction.h"

namespace arm {

CLZInstruction::CLZInstruction(CPURegisters::Register dest, Arg *val, Cond cond)
:
CondInstruction(cond),
dest_(dest),
val_(val)
{
}

CLZInstruction::~CLZInstruction()
{
    delete val_;
}

void CLZInstruction::do_execute(CPURegisters& regs, EndianMemory&) const
{
    std::bitset<32> val(val_->load(regs, false));
    int bits = 0;
    for (; bits < 32 && !val.test(31-bits); ++bits);
    regs.set_reg(dest_, bits);
}

void CLZInstruction::fmt(std::ostream& o) const
{
    o << "CLZ R" << std::dec << dest_ << ", ";
    val_->fmt(o);
}

namespace is {

Instruction *CLZ(CPURegisters::Register dest, CPURegisters::Register val, Cond cond)
{
    return new CLZInstruction(dest, new RegArg(val), cond);
}

}

}
