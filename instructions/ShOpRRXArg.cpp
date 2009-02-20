#include <bitset>

#include "instructions/ShOpRRXArg.h"

ShOpRRXArg::ShOpRRXArg(Arg *val)
:
val_(val)
{
}

ARM_Word ShOpRRXArg::load(CPURegisters& regs, bool) const
{
    std::bitset<32> ret(val_->load(regs, false));
    ProgramStatusRegister& cpsr = regs.status_reg(CPURegisters::CPSR);

    bool old_cbit = cpsr.get_bit(ProgramStatusRegister::C);
    cpsr.set_bit(ProgramStatusRegister::C, ret.test(0));
    ret >>= 1;
    ret.set(31, old_cbit);

    return ARM_Word(ret.to_ulong());
}

void ShOpRRXArg::fmt(std::ostream &o) const
{
    val_->fmt(o);
    o << ", RRX";
}
