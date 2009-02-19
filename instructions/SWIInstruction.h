#ifndef ARMY_CORE_SWIINSTRUCTION_H_
#define ARMY_CORE_SWIINSTRUCTION_H_

#include "instructions/Instruction.h"
#include "instructions/CondInstruction.h"

namespace arm {

class SWIInstruction : public CondInstruction
{
public:
    SWIInstruction(ARM_Word swi_code, Cond cond);
    virtual void do_execute(CPURegisters&, EndianMemory&) const;
    virtual void fmt(std::ostream&) const;

private:
    ARM_Word swi_code_;
};

namespace is {
    Instruction *SWI(ARM_Word swi_code = 0, Cond cond = AL);
}

}

#endif
