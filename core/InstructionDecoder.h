#ifndef ARMY_CORE_INSTRUCTION_DECODER_H_
#define ARMY_CORE_INSTRUCTION_DECODER_H_

#include "armyconfig.h"
#include "core/RuntimeException.h"
#include "memory/EndianMemory.h"

class Memory;
class Instruction;

class UndefinedInstructionException: public RuntimeException {
public:
    UndefinedInstructionException(ARM_Word insn_word_);
    ARM_Word insn_word_;
protected:
    void output_details(std::ostream &o) const;
};

class ARMYCORE_EXPORT InstructionDecoder {
public:
    /**
     * Decodes instruction word from address "insn_addr",
     * and returns decoded instruction representing this word.
     * Caller does not own the object (hence "const"), and has to call "dispose"
     * when done with it
     */
    virtual Instruction *fetch_and_decode(addr_t insn_addr, EndianMemory &mem) = 0;

    /**
     * This must be called after instruction is used
     */
    virtual void dispose(Instruction *insn) = 0;

    /**
     * Tells the instruction decoder that contents of memory from "addr_start"
     * (inclusive) to "addr_end" (exclusive) have changed.
     *
     * This allows decoders that cache things to update their cache, if necessary.
     */
    virtual void invalidate_cache(addr_t addr_start, addr_t addr_end) = 0;

    virtual ~InstructionDecoder(){};

protected:
    InstructionDecoder(){};

private:
    InstructionDecoder(const InstructionDecoder &);
    InstructionDecoder& operator=(const InstructionDecoder &);
};

#endif
