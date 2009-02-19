#ifndef ARMY_CORE_SHOPRRXARG_H_
#define ARMY_CORE_SHOPRRXARG_H_

#include <memory>

#include "instructions/Arg.h"

class ShOpRRXArg : public Arg
{
public:
    ShOpRRXArg(Arg *val);

    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const;
    virtual void fmt(std::ostream&) const;

private:
    std::auto_ptr<Arg> val_;
};

#endif
