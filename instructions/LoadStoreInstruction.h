#ifndef ARMY_CORE_LOADSTOREINSTRUCTION_H_
#define ARMY_CORE_LOADSTOREINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "instructions/Arg.h"

namespace arm {

    enum  TransferType {
        Store,
        Load
    };

    class LoadStoreInstruction : public CondInstruction {
    public:
        LoadStoreInstruction(TransferType transfer_type, CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool write, EndianMemory::Size size = EndianMemory::Word, EndianMemory::Signedness signedness = EndianMemory::Unsigned, Cond cond = AL);
        ~LoadStoreInstruction();

        virtual void do_execute(CPURegisters& regs, EndianMemory& mem) const;
        virtual void fmt(std::ostream&) const;

    private:
        TransferType trans_type_;
        CPURegisters::Register dest_, addr_reg_;
        Arg *offset_;
        bool pre_;
        bool add_;
        bool write_;
        EndianMemory::Size size_;
        EndianMemory::Signedness signedness_;
    };

    namespace is {
        Instruction *LDR(CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool byte, bool write, Cond cond = AL);
        Instruction *STR(CPURegisters::Register dest, CPURegisters::Register addr_reg, Arg *offset, bool pre, bool add, bool byte, bool write, Cond cond = AL);
    }
}

#endif
