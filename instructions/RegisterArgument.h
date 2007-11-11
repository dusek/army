#ifndef __REGISTER_ARGUMENT_H__
#define __REGISTER_ARGUMENT_H__

#include "instructions/Argument.h"
#include "core/CPU.h"
#include "instructions/util.h"

class RegisterArgument: public Argument {
public:
    RegisterArgument(ARM_Register reg): reg_(reg) {}
    virtual void decode(uint32_t insn_word, int start, int end);
    virtual uint32_t read  (CPU &cpu) const;
    virtual void store (CPU &cpu, uint32_t value) const;
    virtual std::string to_string() const;

private:
    ARM_Register reg_;
};

#endif
