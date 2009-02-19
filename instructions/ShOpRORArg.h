#ifndef ARMY_CORE_SHOP_ROR_H_
#define ARMY_CORE_SHOP_ROR_H_

#include <memory>
#include "instructions/Arg.h"

class ShOpRORArg : public Arg
{
public:
    ShOpRORArg(Arg *val, Arg *rot);
    ~ShOpRORArg();

    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const;
    virtual void fmt(std::ostream&) const;

protected:
    std::auto_ptr<Arg> val_;
    std::auto_ptr<Arg> rot_;
};

#endif
