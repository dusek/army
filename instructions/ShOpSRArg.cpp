#include <cassert>
#include <bitset>

#include "instructions/ShOpSRArg.h"

ShOpSRArg::ShOpSRArg(Arg *val, Arg *shift, ShOpSRArg::ShiftType shift_type)
:
ShOpShiftArg(val, shift),
sh_type_(shift_type)
{
}

const char *ShOpSRArg::fmt_op_str() const
{
    const char *fmt_str = 0;

    switch(sh_type_) {
        case ShiftTypeArithmetic: fmt_str = "ASR"; break;
        case ShiftTypeLogical:    fmt_str = "LSR"; break;
        default: assert(!"ShOpSRArg::fmt_op_str: Unhandled ShiftType value");
    }

    return fmt_str;
}

ARM_Word ShOpSRArg::priv_load(CPURegisters &regs, bool update_cpsr) const
{
    ARM_Word val = val_->load(regs, false);
    ARM_Word sh  =  sh_->load(regs, false);
    sh &= 0x000000ff; // get last 8 bits

    ARM_Word ret;
    if (sh == 0) {
        ret = val;
    } else {
        // sh > 0

        if (sh < 32) {
            ret = (val >> sh);
        } else {
            ret = 0;
        }

        if (sh_type_ == ShiftTypeArithmetic && std::bitset<32>(val).test(31)) {
            // extend with ones
            if (sh < 32)
                ret |= ((0xffffffff >> (32 - sh)) << (32 - sh));
            else
                ret = 0xffffffff;
        }
        
        if (update_cpsr) {
            bool cbit;
            if (sh <= 32)
                cbit = std::bitset<32>(val).test(32 - sh); // sh > 0, sh <= 32
            else if (sh_type_ == ShiftTypeArithmetic && std::bitset<32>(val).test(31))
                cbit = true;
            else
                cbit = false;
            regs.status_reg(CPURegisters::CPSR).set_bit(ProgramStatusRegister::C, cbit);
        }
    }

    return ret;
}
