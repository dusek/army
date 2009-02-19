#ifndef ARMY_CORE_SHOPIMMED_ARG_H_
#define ARMY_CORE_SHOPIMMED_ARG_H_

#include "instructions/Arg.h"

class ShOpImmedArg : public Arg
{
public:
    /*
    ** u8 - only lowest 8 bits are taken into account
    ** rot - must be even in interval 0..30 (i.e. 0,2,4,..,28,30)
    **       therefore the last bit is zeroed, and modulo by 32 is taken
    ** value is computed by rotating u8 right by rot bits
    */
    ShOpImmedArg(ARM_Word u8, ARM_Word rot);
    ~ShOpImmedArg();

    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const;

    virtual void fmt(std::ostream&) const;

private:
    ARM_Word val_;
    bool cbit_;
};

#endif
