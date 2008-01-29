#include "core/DecodingInstructionDecoder.h"
#include "instructions/Instruction.h"

#include <algorithm>
#include <cassert>

static void InstructionDeleter(Instruction *i)
{
    delete i;
}

DecodingInstructionDecoder::DecodingInstructionDecoder()
{
    //Add instructions to instruction set here, like this:
    //instruction_set.push_back(new ADDInstruction);
}

class InstuctionMatcher {
public:
    InstuctionMatcher(ARM_Word insn_word): insn_word_(insn_word){};
    bool operator()(Instruction *instruction) {
        return instruction->represented_by(insn_word_);
    };

private:
    ARM_Word insn_word_;
};

const Instruction *DecodingInstructionDecoder::fetch_and_decode(addr_t insn_addr, EndianMemory &mem)
{
    ARM_Word insn_word = mem.read_value(insn_addr);
    instruction_set_t::const_iterator it =
        std::find_if(instruction_set.begin(), instruction_set.end(), InstuctionMatcher(insn_word));
    if (it == instruction_set.end()) {
        throw UndefinedInstructionException(insn_word);
    } else {
        Instruction *ret = (*it)->clone();
        ret->decode(insn_word);
        return ret;
    }
}

void DecodingInstructionDecoder::invalidate_cache(addr_t, addr_t)
{
    //We don't cache anything
}

DecodingInstructionDecoder::~DecodingInstructionDecoder()
{
    std::for_each(instruction_set.begin(), instruction_set.end(), InstructionDeleter);
}
