#include "instructions/CondInstruction.h"

namespace arm {
CondInstruction::CondInstruction(Cond cond)
    :
cond_(cond)
{
}

void CondInstruction::execute(CPURegisters& regs, EndianMemory& mem) const
{
    if (is_cond(regs.status_reg(CPURegisters::CPSR), cond_))
        do_execute(regs, mem);
}

}