#include "instructions/ShOpRORArg.h"

ShOpRORArg::ShOpRORArg(Arg *val, Arg *rot)
:
val_(val),
rot_(rot)
{}

ShOpRORArg::~ShOpRORArg()
{
}

ARM_Word ShOpRORArg::load(CPURegisters &regs, bool update_cpsr) const
{
    ARM_Word val = val_->load(regs, false);
    ARM_Word rot = rot_->load(regs, false);
    rot &= 0x000000ff; // get LSB of rot

    ARM_Word ret;

    if (rot == 0) {
        ret = val;
    } else {
        rot %= 32;
        ret = (val >> rot) | (val << (32 - rot));
        if (update_cpsr) {
            regs.status_reg(CPURegisters::CPSR).set_bit(ProgramStatusRegister::C, (ret & 0x80000000) > 0); //C4800 is insane...
        }
    }

    return ret;
}

void ShOpRORArg::fmt(std::ostream& o) const
{
    val_->fmt(o);
    o << ", ROR ";
    rot_->fmt(o);
}