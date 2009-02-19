#ifndef ARMY_CORE_IMMEDARG_H_
#define ARMY_CORE_IMMEDARG_H_

#include "instructions/Arg.h"

class ImmedArg : public Arg {
public:
    ImmedArg(ARM_Word immed);
    ~ImmedArg();

    ARM_Word val() const;

    virtual ARM_Word load(CPURegisters& regs, bool udpate_cpsr) const;
    virtual void fmt(std::ostream&) const;

private:
    ARM_Word immed_;
};

ImmedArg *ImmedArg_decode(ARM_Word word, std::size_t bit_pos, std::size_t size, bool to_double);
ARM_Word ImmedArg_ValFromWord(ARM_Word word, std::size_t bit_pos, std::size_t size, bool to_double = false);

#endif
