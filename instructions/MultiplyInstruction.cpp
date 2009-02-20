#include <cassert>
#include <bitset>

#include "instructions/MultiplyInstruction.h"

#include "instructions/RegArg.h"
#include "armyconfig.h"

namespace arm {

MultiplyInstruction::MultiplyInstruction(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, Arg *op1, Arg *op2, bool accumulate, EndianMemory::Signedness signedness, bool long_, bool update_cpsr, Cond cond)
:
CondInstruction(cond),
dest_lo_(dest_lo),
dest_hi_(dest_hi),
op1_(op1),
op2_(op2),
accumulate_(accumulate),
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
    if (signedness_ == EndianMemory::Signed) {
        std::cerr << "!!! Signed multiply not implemented !!!" << std::endl;
        return;
    }

    uint64_t op1 = op1_->load(regs, false);
    uint64_t op2 = op2_->load(regs, false);

    uint64_t result = op1 * op2;

    if (accumulate_) {
        uint64_t dest_hi_val = regs.get_reg(dest_hi_) + ((dest_hi_ == CPURegisters::PC) ? 8 : 0);
        uint64_t dest_lo_val = regs.get_reg(dest_lo_) + ((dest_lo_ == CPURegisters::PC) ? 8 : 0);
        if (!long_) {
            result += dest_hi_val;
        } else {
            result += dest_lo_val;
            result += dest_hi_val << 32;
        }
    }

    uint32_t result_lo = result & 0xffffffff;
    uint32_t result_hi = result >> 32;

    regs.set_reg(dest_lo_, result_lo);
    if (long_)
        regs.set_reg(dest_hi_, result_hi);

    if (update_cpsr_) {
        bool zbit = false;
        ProgramStatusRegister& cpsr = regs.status_reg(CPURegisters::CPSR);
        if (result == 0) {
            zbit = true;
        }
        cpsr.set_bit(ProgramStatusRegister::Z, zbit);

        bool nbit = false;
        if ((result >> 63) == 1) {
            nbit = true;
        }
        cpsr.set_bit(ProgramStatusRegister::N, nbit);
    }
}

void MultiplyInstruction::fmt(std::ostream& o) const
{
    if (long_) {
        if (signedness_ == EndianMemory::Signed)
            o << "S";
        else
            o << "U";
    }

    if (accumulate_)
        o << "MLA";
    else
        o << "MUL";

    if (long_)
        o << "L";
    o << " ";

    if (cond_ != AL)
        o << cond_;

    if (update_cpsr_)
        o << "S";

    o << std::dec;

    o << "R" << std::dec << dest_lo_ << ", ";
    if (long_)
        o << "R" << std::dec << dest_hi_ << ", ";

    op1_->fmt(o);
    o << ", ";
    op2_->fmt(o);

    if (!long_ && accumulate_)
        o << "R" << std::dec << dest_hi_;
}

namespace is {
Instruction *MULL(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, CPURegisters::Register op1, CPURegisters::Register op2, bool accumulate, EndianMemory::Signedness signedness, bool S, Cond cond)
{
    return new MultiplyInstruction(dest_lo, dest_hi, new RegArg(op1), new RegArg(op2), accumulate, signedness, true, S, cond);
}

Instruction *MUL(CPURegisters::Register dest, CPURegisters::Register acc, CPURegisters::Register op1, CPURegisters::Register op2, bool accumulate, bool S, Cond cond)
{
    return new MultiplyInstruction(dest, acc, new RegArg(op1), new RegArg(op2), accumulate, EndianMemory::Unsigned, false, S, cond);
}

}

}
