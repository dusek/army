#ifndef ARMY_CORE_SHOPSRARG_H_
#define ARMY_CORE_SHOPSRARG_H_

#include "instructions/ShOpShiftArg.h"

class ShOpSRArg : public ShOpShiftArg
{
public:
    enum ShiftType {
        ShiftTypeLogical,
        ShiftTypeArithmetic
    };

    ShOpSRArg(Arg *val, Arg *shift, ShiftType shift_type);

private:
    const char *fmt_op_str() const;
    virtual ARM_Word priv_load(CPURegisters& regs, bool update_cpsr) const;

    ShiftType sh_type_;
};

#endif
