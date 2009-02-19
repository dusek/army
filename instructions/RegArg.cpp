#include <cassert>

#include "instructions/RegArg.h"

RegArg::RegArg(CPURegisters::Register reg)
: reg_(reg)
{}

ARM_Word RegArg::load(CPURegisters &regs, bool) const
{
    ARM_Word ret = regs.get_reg(reg_);
    if (reg_ == CPURegisters::R15)
        ret += 8;
    return ret;
}

void RegArg::fmt(std::ostream &o) const
{
    o << "R" << std::dec << reg_;
}

void RegArg::store(CPURegisters& regs, ARM_Word val) const
{
    regs.set_reg(reg_, val);
}

CPURegisters::Register RegArg::reg() const
{
    return reg_;
}

RegArg *RegArg_decode(ARM_Word word, std::size_t bit_pos)
{
    CPURegisters::Register reg = RegisterFromWord(word, bit_pos);
    return new RegArg(reg);
}

RegArg::~RegArg()
{}
