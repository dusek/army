#include <bitset>

#include "instructions/LoadStoreMultipleInstruction.h"

namespace arm {

LoadStoreMultipleInstruction::LoadStoreMultipleInstruction(CPURegisters::Register addr, bool P, bool U, bool W, ARM_Word reg_mask, TransferType transfer_type, Cond cond)
:
CondInstruction(cond),
trans_type_(transfer_type),
inclusive_(P),
increment_(U),
write_back_(W),
reg_mask_(reg_mask),
addr_reg_(addr)
{
}

void LoadStoreMultipleInstruction::do_execute(CPURegisters &regs, EndianMemory &mem) const
{
    ARM_Word addr = regs.get_reg(addr_reg_);
    if (addr_reg_ == CPURegisters::PC)
        addr += 8;
    ARM_Word addr_step = increment_ ? 4 : -4;
    if (!inclusive_)
        addr += addr_step;

    std::bitset<32> reg_mask(reg_mask_);
    for (int i = 0; i < 16; ++i) {
        if (reg_mask.test(i)) {
            if (trans_type_ == Load)
                regs.set_reg(CPURegisters::Register(i), mem.read_value(addr));
            else {
                ARM_Word reg_value = regs.get_reg(CPURegisters::Register(i));
                if (i == 15)
                    reg_value += 8;
                mem.write_value(addr, reg_value);
            }
            addr += addr_step;
        }
    }

    if (write_back_) {
        if (!inclusive_)
            addr -= addr_step;
        regs.set_reg(addr_reg_, addr);
    }
}

void LoadStoreMultipleInstruction::fmt(std::ostream& o) const
{
    if (trans_type_ == Load)
        o << "LDM";
    else
        o << "STM";
    
    if (cond_ != AL)
        o << cond_;

    if (increment_)
        o << "I";
    else
        o << "D";

    if (inclusive_)
        o << "A";
    else
        o << "B";

    o << " R" << std::dec << addr_reg_;

    if (write_back_)
        o << "!";

    o << " {";
    
    bool first = true;
    std::bitset<32> reg_mask(reg_mask_);
    for (int i = 0; i < 16; ++i) {
        if (reg_mask.test(i)) {
            if (!first)
                o << ", ";
            else
                first = false;
            o << "R" << std::dec << i;
        }
    }

    o << "}";
}

namespace is {

Instruction *LDM(bool inclusive, bool increment, bool write_back, CPURegisters::Register addr_reg, ARM_Word reg_mask, Cond cond)
{
    return new LoadStoreMultipleInstruction(addr_reg, inclusive, increment, write_back, reg_mask, Load, cond);
}

Instruction *STM(bool inclusive, bool increment, bool write_back, CPURegisters::Register addr_reg, ARM_Word reg_mask, Cond cond)
{
    return new LoadStoreMultipleInstruction(addr_reg, inclusive, increment, write_back, reg_mask, Store, cond);
}

}

}