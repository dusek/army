#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <string>

#include "core/CPU.h"
#include "platform/pstdint.h"

class Argument {
public:

    /**
     * Decodes this argument based on the instruction word, and start end end
     * bits within this instruction word.
     */
    virtual void decode(uint32_t insn_word, int start, int end) = 0;

    /**
     * Reads this argument from specified CPU.
     */
    virtual uint32_t read  (CPU &cpu) const = 0;

    /**
     * Stores the value "value" to the specified CPU to the location specified
     * by this argument
     */
    virtual void store (CPU &cpu, uint32_t value) const = 0;

    /**
     * Returns text (assembly-like) representation of this argument.
     */
    virtual std::string to_string() const = 0;

};

#endif
