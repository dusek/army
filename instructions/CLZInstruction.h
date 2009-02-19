#ifndef ARMY_CORE_CLZINSTRUCTION_H_
#define ARMY_CORE_CLZINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "instructions/RegArg.h"

namespace arm {
    class CLZInstruction : public CondInstruction {
    public:
        CLZInstruction(CPURegisters::Register dest, Arg *val, Cond cond);
        ~CLZInstruction();

        virtual void do_execute(CPURegisters& regs, EndianMemory&) const;
        virtual void fmt(std::ostream& o) const;

    private:
        CPURegisters::Register dest_;
        Arg *val_;
    };

    namespace is {
        Instruction *CLZ(CPURegisters::Register dest, CPURegisters::Register val, Cond cond = AL);
    }
}

#endif
