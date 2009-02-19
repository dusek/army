#ifndef ARMY_CORE_ARG_H_
#define ARMY_CORE_ARG_H_

#include "armyconfig.h"
#include "core/CPURegisters.h"
#include <ostream>

class ARMYCORE_EXPORT Arg {
public:

protected:
    Arg();

public:
    virtual ~Arg() = 0;
    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const = 0;
    virtual void fmt(std::ostream&) const = 0;
};

Arg *Arg_decode_shifter_operand(ARM_Word insn_word);
Arg *Arg_decode_ldst_index(ARM_Word insn_word);

std::ostream& operator<<(std::ostream&, const Arg&);

#endif
