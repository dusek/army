#ifndef __IMMEDIATE_ARGUMENT_H__
#define __IMMEDIATE_ARGUMENT_H__

#include "Argument.h"

#include "core/CPU.h"
#include "platform/pstdint.h"

class ImmediateArgument: public Argument {
public:
    ImmediateArgument(uint32_t value): value_(value) {}
    virtual void        decode(uint32_t insn_word, int start, int end);
    virtual uint32_t    read(CPU &cpu) const;
    virtual void        store(CPU &cpu, uint32_t value) const;
    virtual std::string to_string() const;

private:
    uint32_t value_;
};

#endif
