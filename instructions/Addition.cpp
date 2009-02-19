#include "instructions/Addition.h"

#include "instructions/RegArg.h"

namespace arm {

Addition::Addition(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool has_carry)
: DPInstruction(cond, dest_reg, op1, sh_arg, update_cpsr, store_result, false, false, has_carry, false)
{
}

ARM_Word Addition::compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const
{
    ARM_Word carry_value = carry ? 1u : 0u;
    ARM_Word result = op1 + op2 + carry_value;

    if (update_cv) {
        bool cbit = false;
        if (op2 == 0xffffffff && carry_value == 1)
            cbit = true;
        else
            cbit = result < op1;
        cpsr.set_bit(ProgramStatusRegister::C, cbit);

        bool vbit = false;
#define NEG(a) (((a) >> 31) == 1)
#define POS(a) (((a) >> 31) == 0)
        if (NEG(result) && POS(op1) && POS(op2))
            vbit = true;
        if (POS(result) && NEG(op1) && NEG(op2))
            vbit = true;
        cpsr.set_bit(ProgramStatusRegister::V, cbit);
    }

    return result;
}

const char *Addition::mnemonic() const
{
    if (!store_result_)
        return "CMN";
    else if (has_carry_)
        return "ADC";
    else
        return "ADD";
}

namespace is {
Instruction *ADD(CPURegisters::Register dest_reg, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new Addition(cond, dest_reg, new RegArg(op1), sh_op2, S, true, false);
}

Instruction *ADC(CPURegisters::Register dest_reg, CPURegisters::Register op1, Arg *sh_op2, bool S, Cond cond)
{
    return new Addition(cond, dest_reg, new RegArg(op1), sh_op2, S, true, true);
}

Instruction *CMN(CPURegisters::Register op1, Arg *sh_op2, Cond cond)
{
    return new Addition(cond, CPURegisters::R0, new RegArg(op1), sh_op2, true, false, false);
}

} //ns is

} //ns arm
