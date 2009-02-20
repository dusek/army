#include "instructions/ShOpShiftArg.h"

ShOpShiftArg::ShOpShiftArg(Arg *val, Arg *shift)
:
val_(val),
sh_(shift)
{
}

void ShOpShiftArg::fmt(std::ostream& o) const
{
    val_->fmt(o);
    o << ", " << fmt_op_str() << " ";
    sh_->fmt(o);
}

ARM_Word ShOpShiftArg::load(CPURegisters& regs, bool update_cpsr) const
{
    return priv_load(regs, update_cpsr);
}

ShOpShiftArg::~ShOpShiftArg()
{
}
