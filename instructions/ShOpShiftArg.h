#ifndef ARMY_CORE_SHOPSHIFTARG_H_
#define ARMY_CORE_SHOPSHIFTARG_H_

#include "instructions/Arg.h"
#include <memory>

class ShOpShiftArg : public Arg
{
public:
    ~ShOpShiftArg();
    virtual ARM_Word load(CPURegisters& regs, bool udpate_cpsr) const;
    virtual void fmt(std::ostream&) const;

protected:
    ShOpShiftArg(Arg *val, Arg *shift);
    std::auto_ptr<Arg> val_;
    std::auto_ptr<Arg> sh_;

private:
    virtual const char *fmt_op_str() const = 0;
    virtual ARM_Word priv_load(CPURegisters& regs, bool udpate_cpsr) const = 0;
};

#endif
