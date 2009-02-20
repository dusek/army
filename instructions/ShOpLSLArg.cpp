#include <bitset>

#include "instructions/ShOpLSLArg.h"

ShOpLSLArg::ShOpLSLArg(Arg *val, Arg *shift)
:
ShOpShiftArg(val, shift)
{
}

ShOpLSLArg::~ShOpLSLArg()
{
}

ARM_Word ShOpLSLArg::priv_load(CPURegisters &regs, bool update_cpsr) const
{
    ARM_Word val = val_->load(regs, false);
    ARM_Word sh  =  sh_->load(regs, false);
    sh &= 0x000000ff;

    ARM_Word ret;

    if (sh == 0) {
        ret = val;
    } else {
        // sh > 0

        if (sh < 32)
            ret = val << sh;
        else
            ret = 0;

        if (update_cpsr) {
            bool cbit;

            if (sh <= 32)
                cbit = std::bitset<32>(val).test(32 - sh); // sh > 0
            else
                cbit = false;

            regs.status_reg(CPURegisters::CPSR).set_bit(ProgramStatusRegister::C, cbit);
        }
    }

    return ret;
}

const char *ShOpLSLArg::fmt_op_str() const
{
    return "LSL";
}
