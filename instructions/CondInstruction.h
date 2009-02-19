#ifndef ARMY_CORE_CONDINSTRUCTION_H_
#define ARMY_CORE_CONDINSTRUCTION_H_

#include "instructions/cond.h"
#include "instructions/Instruction.h"

namespace arm {
    class CondInstruction : public Instruction
    {
    public:
        virtual void execute(CPURegisters& regs, EndianMemory& mem) const;

    protected:
        CondInstruction(Cond cond);
        virtual void do_execute(CPURegisters& regs, EndianMemory& mem) const = 0;

        Cond cond_;
    };
}

#endif
