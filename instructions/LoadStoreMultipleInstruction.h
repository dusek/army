#ifndef ARMY_CORE_LOADSTOREMULTIPLEINSTRUCTION_H_
#define ARMY_CORE_LOADSTOREMULTIPLEINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "instructions/LoadStoreInstruction.h"

namespace arm {

    class LoadStoreMultipleInstruction : public CondInstruction {
    public:
        LoadStoreMultipleInstruction(CPURegisters::Register addr, bool P, bool U, bool W, ARM_Word reg_mask, TransferType transfer_type, Cond cond = AL);

        virtual void do_execute(CPURegisters& regs, EndianMemory& mem) const;
        virtual void fmt(std::ostream&) const;

    private:
        TransferType trans_type_;
        bool inclusive_;
        bool increment_;
        bool write_back_;
        ARM_Word reg_mask_;
        CPURegisters::Register addr_reg_;
    };

    namespace is {
        Instruction *LDM(bool inclusive, bool increment, bool write_back, CPURegisters::Register addr_reg, ARM_Word reg_mask, Cond cond = AL);
        Instruction *STM(bool inclusive, bool increment, bool write_back, CPURegisters::Register addr_reg, ARM_Word reg_mask, Cond cond = AL);
    }

}

#endif
