#include "instructions/DPInstruction.h"
#include <bitset>
#include <algorithm>

namespace arm {

DPInstruction::DPInstruction(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool sh_updates_c, bool reverse_args, bool has_carry, bool default_carry)
:
CondInstruction(cond),
store_result_(store_result),
update_cpsr_(update_cpsr),
reverse_args_(reverse_args),
sh_updates_c_(sh_updates_c),
has_carry_(has_carry),
default_carry_(default_carry),
dest_reg_(dest_reg),
sh_arg_(sh_arg),
op1_(op1)
{
}

void DPInstruction::do_execute(CPURegisters &regs, EndianMemory &) const
{
    bool psr_override = false;
    if (store_result_ && dest_reg_ == CPURegisters::PC)
        psr_override = true;

    bool do_update_cpsr = update_cpsr_ && !psr_override;

    ProgramStatusRegister& cpsr = regs.status_reg(CPURegisters::CPSR);

    bool cur_carry = has_carry_ ? cpsr.get_bit(ProgramStatusRegister::C) : default_carry_;
    ARM_Word op1 = 0;
    if (op1_)
        op1 = op1_->load(regs, false);
    ARM_Word op2 = sh_arg_->load(regs, do_update_cpsr && sh_updates_c_);

    if (reverse_args_)
        std::swap(op1, op2);

    ARM_Word result = compute_result(op1, op2, do_update_cpsr, cur_carry, cpsr);
    if (do_update_cpsr) {
        cpsr.set_bit(ProgramStatusRegister::Z, result == 0);
        cpsr.set_bit(ProgramStatusRegister::N, (result & 0x80000000) != 0);
    }

    // somehow arm-elf-gcc in its libgcc.a, in __aeabi_uidiv, has "ADDNE   PC, PC, R3, LSL #2"
    // so it assumes adding to PC works --> for practical reasons,
    // disable the below correctness (we don't need Priviledged mode anyway)
    //if (psr_override)
    //    cpsr = regs.status_reg(CPURegisters::SPSR);

    if (store_result_)
        regs.set_reg(dest_reg_, result);
}

void DPInstruction::fmt(std::ostream& o) const
{
    o << mnemonic();
    if (cond_ != AL && cond_ != NV)
        o << cond_;
    if (store_result_ && update_cpsr_)
        o << "S";
    o << " ";
    bool first = true;
    if (store_result_) {
        first = false;
        o << "R" << std::dec << dest_reg_;
    }

    if (op1_) {
        if (!first)
            o << ", ";
        first = false;
        op1_->fmt(o);
    }

    if (!first)
        o << ", ";
    sh_arg_->fmt(o);
}

DPInstruction::~DPInstruction()
{
    delete sh_arg_;
    delete op1_;
}

}