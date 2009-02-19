#ifndef ARMY_CORE_REGARG_H_
#define ARMY_CORE_REGARG_H_

#include "instructions/Arg.h"

class RegArg : public Arg {
public:
    explicit RegArg(CPURegisters::Register reg);
    ~RegArg();

    virtual ARM_Word load(CPURegisters& regs, bool update_cpsr) const;
    virtual void store(CPURegisters& regs, ARM_Word val) const;
    CPURegisters::Register reg() const;

    virtual void fmt(std::ostream&) const;

private:
    CPURegisters::Register reg_;
};

RegArg *RegArg_decode(ARM_Word word, std::size_t bit_pos);

#endif
