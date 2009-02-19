#include <bitset>
#include <memory>

#include "instructions/BranchInstruction.h"

#include "instructions/ImmedArg.h"
#include "instructions/RegArg.h"

namespace arm {

BranchInstruction::BranchInstruction(Arg *target, bool link, bool exchange, Cond cond)
:
CondInstruction(cond),
tgt_(target),
link_(link),
exchange_(exchange)
{
}

BranchInstruction::~BranchInstruction()
{
    delete tgt_;
}

class OffsetAddrArg : public Arg {
public:
    OffsetAddrArg(ARM_Word offset): offset_(offset) {};
    ARM_Word load(CPURegisters& regs, bool) const {
        ARM_Word pc = regs.get_reg(CPURegisters::PC) + 8;
        return pc + offset_;
    }

    void fmt(std::ostream& o) const
    {
        o << '#' << std::dec << (int)offset_;
    }

private:
    ARM_Word offset_;
};

void BranchInstruction::do_execute(CPURegisters& regs, EndianMemory&) const
{
    ARM_Word tgt_addr = tgt_->load(regs, false);

    if (link_) {
        ARM_Word ret_addr = regs.get_reg(CPURegisters::PC) + 4;
        regs.set_reg(CPURegisters::LR, ret_addr);
    }

    regs.set_reg(CPURegisters::PC, tgt_addr);
}

void BranchInstruction::fmt(std::ostream& o) const
{
    o << "B";
    if (exchange_)
        o << "X";
    if (link_)
        o << "L";
    if (cond_ != AL && cond_ != NV)
        o << cond_;
    o << " ";
    tgt_->fmt(o);
}

namespace is {

Instruction *B(ARM_Word offset, bool link, Cond cond)
{
    // sign-extend if needed
    if (std::bitset<32>(offset).test(23))
        offset |= 0xff000000;
    offset <<= 2;

    return new BranchInstruction(new OffsetAddrArg(offset), link, false, cond);
}

Instruction *BX(CPURegisters::Register target_reg, Cond cond)
{
    return new BranchInstruction(new RegArg(target_reg), false, true, cond);
}

}

}