#ifndef ARMY_CORE_SHOPLSRARG_H_
#define ARMY_CORE_SHOPLSRARG_H_

#include "instructions/ShOpShiftArg.h"

class ShOpLSRArg : public ShOpShiftArg
{
public:
    ShOpLSRArg(Arg *val, Arg *shift);
    virtual ~ShOpLSR();

    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const;
    virtual void fmt(std::ostream&) const;

private:
    const char *fmt_op_str() const;
};

#endif