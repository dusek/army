#ifndef ARMY_CORE_MULTIPLYINSTRUCTION_H_
#define ARMY_CORE_MULTIPLYINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "instructions/Arg.h"

namespace arm {

class MultiplyInstruction : public CondInstruction
{
public:
    MultiplyInstruction(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, Arg *op1, Arg *op2, EndianMemory::Signedness signedness, bool long_, bool update_cpsr, Cond cond = AL);
    ~MultiplyInstruction();

    virtual void do_execute(CPURegisters&, EndianMemory&) const;
    virtual void fmt(std::ostream&) const;

private:
    CPURegisters::Register dest_lo_;
    CPURegisters::Register dest_hi_;
    Arg *op1_;
    Arg *op2_;
    EndianMemory::Signedness signedness_;
    bool long_;
    bool update_cpsr_;
};

namespace is {
    Instruction *UMULL(CPURegisters::Register dest_lo, CPURegisters::Register dest_hi, CPURegisters::Register op1, CPURegisters::Register op2, bool S, Cond cond = AL);
    Instruction *MUL(CPURegisters::Register dest, CPURegisters::Register op1, CPURegisters::Register op2, bool S, Cond cond = AL);
}

}

#endif
