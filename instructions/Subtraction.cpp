#include "instructions/Subtraction.h"

#include "instructions/RegArg.h"

namespace arm {

Subtraction::Subtraction(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool reverse_args, bool has_carry)
: DPInstruction(cond, dest_reg, op1, sh_arg, update_cpsr, store_result, false, reverse_args, has_carry, true)
{
}

ARM_Word Subtraction::compute_result(ARM_Word op1, ARM_Word op2, bool update_cv, bool carry, ProgramStatusRegister& cpsr) const
{
    ARM_Word carry_value = (carry ? 0 : 1);
    ARM_Word result = op1 - op2 - carry_value;

    if (update_cv) {
        // update C
        bool cbit = (op2 == 0xffffffff) && (carry_value == 1);
        if (!cbit)
            cbit = result > op1;
        cpsr.set_bit(ProgramStatusRegister::C, cbit);

        // update V
        bool vbit = false;
#define NEG(a) (((a) >> 31) == 1)
#define POS(a) (((a) >> 31) == 0)
        if (NEG(result) && POS(op1) && NEG(op2))
            vbit = true;
        if (POS(result) && NEG(op1) && POS(op2))
            vbit = true;
        cpsr.set_bit(ProgramStatusRegister::V, vbit);
    }

    return result;
}

const char *Subtraction::mnemonic() const
{
    const char *ret = 0;
    // CMP
    if (!store_result_)
        ret = "CMP";
    else {
        if (has_carry_)
        {
            if (reverse_args_)
                ret = "RSC";
            else
                ret = "SBC";
        } else {
            if (reverse_args_)
                ret = "RSB";
            else
                ret = "SUB";
        }
    }

    return ret;
}

namespace is {
Instruction *SUB(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S, Cond cond)
{
    return new Subtraction(cond, dest, new RegArg(op1), sh_op, S, true, false, false);
}

Instruction *SBC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S, Cond cond)
{
    return new Subtraction(cond, dest, new RegArg(op1), sh_op, S, true, false, true);
}

Instruction *RSB(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S, Cond cond)
{
    return new Subtraction(cond, dest, new RegArg(op1), sh_op, S, true, true, false);
}

Instruction *RSC(CPURegisters::Register dest, CPURegisters::Register op1, Arg *sh_op, bool S, Cond cond)
{
    return new Subtraction(cond, dest, new RegArg(op1), sh_op, S, true, true, true);
}

Instruction *CMP(CPURegisters::Register op1, Arg *sh_op, Cond cond)
{
    return new Subtraction(cond, CPURegisters::R0, new RegArg(op1), sh_op, true, false, false, false);
}

}

}