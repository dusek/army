#ifndef ARMY_CORE_BRANCHINSTRUCTION_H_
#define ARMY_CORE_BRANCHINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "instructions/Arg.h"
#include "instructions/ImmedArg.h"

namespace arm {
    class BranchInstruction : public CondInstruction {
    public:
        BranchInstruction(Arg *target, bool link, bool exchange, Cond cond);
        ~BranchInstruction();

    protected:
        virtual void fmt(std::ostream&) const;
        virtual void do_execute(CPURegisters& regs, EndianMemory& mem) const;

    private:
        Arg *tgt_;
        bool link_;
        bool exchange_;
    };

    namespace is {
        Instruction *B(ARM_Word offset, bool link = false, Cond cond = AL);
        Instruction *BX(CPURegisters::Register target_reg, Cond cond = AL);
    }
}

#endif
