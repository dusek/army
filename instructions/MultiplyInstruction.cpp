#include <cassert>
#include <bitset>

#include "instructions/MultiplyInstruction.h"

#include "instructions/RegArg.h"
#include "armyconfig.h"

namespace arm {

MultiplyInstruction::MultiplyInstruction(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, Arg *op1, Arg *op2, EndianMemory::Signedness signedness, bool long_, bool update_cpsr, Cond cond)
:
CondInstruction(cond),
dest_lo_(dest_lo),
dest_hi_(dest_hi),
op1_(op1),
op2_(op2),
signedness_(signedness),
long_(long_),
update_cpsr_(update_cpsr)
{
}

MultiplyInstruction::~MultiplyInstruction()
{
    delete op1_;
    delete op2_;
}

void MultiplyInstruction::do_execute(CPURegisters& regs, EndianMemory&) const
{
    uint64_t op1 = op1_->load(regs, false);
    uint64_t op2 = op2_->load(regs, false);

    uint64_t result = op1 * op2;

    uint32_t result_lo = result & 0xffffffff;
    uint32_t result_hi = result >> 32;

    regs.set_reg(dest_lo_, result_lo);
    if (long_)
        regs.set_reg(dest_hi_, result_hi);
}

void MultiplyInstruction::fmt(std::ostream& o) const
{
    if (long_) {
        if (signedness_ == EndianMemory::Signed)
            o << "S";
        else
            o << "U";
    }

    o << "MUL";
    if (long_)
        o << "L";
    o << " ";

    if (cond_ != AL)
        o << cond_;

    if (update_cpsr_)
        o << "S";

    o << std::dec;

    o << "R" << dest_lo_ << ", ";
    if (long_)
        o << "R" << dest_hi_ << ", ";

    op1_->fmt(o);
    o << ", ";
    op2_->fmt(o);
}

namespace is {
Instruction *UMULL(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, CPURegisters::Register op1, CPURegisters::Register op2, bool S, Cond cond)
{
    return new MultiplyInstruction(dest_lo, dest_hi, new RegArg(op1), new RegArg(op2), EndianMemory::Unsigned, true, S, cond);
}

Instruction *MUL(CPURegisters::Register dest, CPURegisters::Register op1, CPURegisters::Register op2, bool S, Cond cond)
{
    return new MultiplyInstruction(dest, CPURegisters::R0, new RegArg(op1), new RegArg(op2), EndianMemory::Unsigned, false, S, cond);
}

}

}