#include "core/InstructionDecoder.h"

UndefinedInstructionException::UndefinedInstructionException(ARM_Word insn_word)
: insn_word_(insn_word)
{}

void UndefinedInstructionException::output_details(std::ostream &o) const
{
    o << "Unknown instruction represented by word " << std::ios_base::hex << insn_word_ << std::ios_base::dec << std::endl;
}