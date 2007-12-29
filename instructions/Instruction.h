#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <string>
#include <vector>

#include "platform/pstdint.h"
#include "core/CPU.h"
#include "instructions/Argument.h"

class Instruction {
public:

    /**
     * Returns true if instruction word \a insn_word represents binary form
     * of this instruction.
     */
    virtual bool represented_by(uint32_t insn_word) const;

    /**
     * Decodes instruction word \a insn_word and stores the results in this
     * instruction instance.
     */
    virtual void decode(uint32_t insn_word) = 0;

    /**
     * Returns if this instruction is executable based on its condition code
     */
    virtual bool executable(CPU &cpu) const;

    /**
     * Executes this instruction on the supplied CPU
     */
    virtual void execute(CPU &cpu) const = 0;

    /**
     * Returns text representation of this instruction in assembly-like language
     */
    virtual std::string to_string(bool verbose) const;

    virtual ~Instruction();

protected:

    /**
     * Returns a decoding mask specifying which bits returned by bits()
     * must be identical with corresponding bits of instruction word so that
     * the instruction word is binary form of this instruction.
     */
    virtual uint32_t mask() const = 0;

    /**
     * Mnemonic representing this instruction in assembly-like language.
     */
    virtual std::string mnemonic() const = 0;

    /**
     * Returns stringified mnemonic qualifier
     */
    virtual std::string str_qualifiers() const = 0;

    /**
     * Returns the string representation of "executable" mnemonic qualifier
     */
    virtual std::string str_condition_code() const;

    /**
     * \see mask()
     */
    virtual uint32_t bits() const = 0;

protected:
    std::vector<Argument *> args;
    std::bitset<4> condition_code;
};

#endif
