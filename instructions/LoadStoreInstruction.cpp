#include "instructions/LoadStoreInstruction.h"

namespace arm {

LoadStoreInstruction::LoadStoreInstruction(TransferType transfer_type, CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool write, EndianMemory::Size size, EndianMemory::Signedness signedness, Cond cond)
:
CondInstruction(cond),
trans_type_(transfer_type),
dest_(dest),
addr_reg_(addr_reg),
offset_(offset),
pre_(pre),
add_(add),
write_(write),
size_(size),
signedness_(signedness)
{
}

LoadStoreInstruction::~LoadStoreInstruction()
{
    delete offset_;
}

void LoadStoreInstruction::do_execute(CPURegisters& regs, EndianMemory& mem) const
{
    ARM_Word offset = offset_->load(regs, false);
    ARM_Word base = regs.get_reg(addr_reg_);
    ARM_Word addr;
    if (!add_)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4146)
#endif
        offset = -offset;
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    if (pre_) {
        addr = base + offset;
    } else {
        addr = base;
    }

    ARM_Word value;
    if (trans_type_ == Load)
        value = mem.read_value(addr, size_, signedness_);
    else
        value = regs.get_reg(dest_);

    if (!pre_)
        addr += offset;
    if (!pre_ || write_) {
        regs.set_reg(addr_reg_, addr);
    }

    if (trans_type_ == Load)
        regs.set_reg(dest_, value);
    else
        mem.write_value(addr, value, size_);
}

void LoadStoreInstruction::fmt(std::ostream& o) const
{
    if (trans_type_ == Load)
        o << "LDR";
    else
        o << "STR";

    if (cond_ != AL && cond_ != NV)
        o << cond_;

    if (signedness_ == EndianMemory::Signed)
        o << "S";

    if (size_ == EndianMemory::Byte)
        o << "B";
    else if (size_ == EndianMemory::Halfword)
        o << "H";

    if (!pre_ && write_)
        o << "T";

    o << " R" << std::dec << dest_ << ", [R" << std::dec << addr_reg_;
    
    if (!pre_)
        o << "]";
    
    o << ", ";
    if (add_)
        o << "+";
    else
        o << "-";

    offset_->fmt(o);
    
    if (pre_) {
        o << "]";
        if (write_)
            o << "!";
    }
}

namespace is {
Instruction *LDR(CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool byte, bool write, Cond cond)
{
    return new LoadStoreInstruction(Load, dest, addr_reg, offset, pre, add, write, byte ? EndianMemory::Byte : EndianMemory::Word, EndianMemory::Unsigned, cond);
}

Instruction *STR(CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool byte, bool write, Cond cond)
{
    return new LoadStoreInstruction(Store, dest, addr_reg, offset, pre, add, write, byte ? EndianMemory::Byte : EndianMemory::Word, EndianMemory::Unsigned, cond);
}

}
}