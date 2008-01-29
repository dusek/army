#ifndef ARMY_CORE_INSTRUCTION_H_
#define ARMY_CORE_INSTRUCTION_H_

#include <ostream>

#include "armyconfig.h"
#include "core/CPURegisters.h"
#include "memory/EndianMemory.h"

class Instruction {
public:

    /**
     * Returns true if instruction word \a insn_word represents binary form
     * of this instruction.
     */
    virtual bool represented_by(ARM_Word insn_word) const = 0;

    /**
     * Decodes instruction word \a insn_word and stores the results in this
     * instruction instance.
     */
    virtual void decode(ARM_Word insn_word) = 0;

    /**
     * Returns if this instruction is executable based on its condition code
     */
    virtual bool executable(const CPURegisters &regs) const = 0;

    /**
     * Executes this instruction on the supplied CPU
     */
    virtual void execute(CPURegisters &regs, EndianMemory &mem) const = 0;

    /**
     * Prints text representation of this instruction in assembly-like language
     */
    virtual void print(std::ostream &o) const = 0;

    /**
     * Returns exact copy of this instruction
     */
    virtual Instruction *clone() const = 0;

    virtual ~Instruction() = 0;
};

ARMYCORE_EXPORT std::ostream &operator<<(std::ostream &o, const Instruction &instruction);

#endif
