#ifndef ARMY_CORE_SHOPLSLARG_H_
#define ARMY_CORE_SHOPLSLARG_H_

#include "instructions/ShOpShiftArg.h"

class ShOpLSLArg : public ShOpShiftArg
{
public:
    ShOpLSLArg(Arg *val, Arg *shift);
    virtual ~ShOpLSLArg();

private:
    const char *fmt_op_str() const;
    virtual ARM_Word priv_load(CPURegisters& regs, bool update_cpsr) const;
};

#endif
