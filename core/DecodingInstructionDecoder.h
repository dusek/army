#ifndef ARMY_CORE_DECODING_INSTRUCTION_DECODER_H_
#define ARMY_CORE_DECODING_INSTRUCTION_DECODER_H_

#include <vector>

#include "core/InstructionDecoder.h"

class Instruction;

class DecodingInstructionDecoder : public InstructionDecoder {
public:
    DecodingInstructionDecoder();

    virtual Instruction *fetch_and_decode(addr_t insn_addr, EndianMemory &mem);
    virtual void dispose(Instruction *insn);
    virtual void invalidate_cache(addr_t insn_addr_start, addr_t insn_addr_end);
    ~DecodingInstructionDecoder();

private:
    DecodingInstructionDecoder(const DecodingInstructionDecoder &);
    DecodingInstructionDecoder &operator=(const DecodingInstructionDecoder &);

    typedef std::vector<Instruction *> instruction_set_t;
    instruction_set_t instruction_set;
};

#endif
