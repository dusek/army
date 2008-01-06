#include "instructions/Instruction.h"

bool Instruction::represented_by(uint32_t insn_word) const
{
    return (insn_word & mask()) == (bits() & mask());
}

std::string Instruction::to_string(bool) const
{
    std::string ret;

    ret += mnemonic();
    ret += str_qualifiers();
    ret += str_condition_code();
    ret += " ";
    std::vector<Argument *>::const_iterator it;
    bool first = true;

    for (it = args.begin(); it != args.end(); ++it) {
        if (first)
            first = false;
        else
            ret += ", ";
        ret += (*it)->to_string();
    }

    return ret;
}

bool Instruction::executable(CPU &) const {
    //FIXME stub
    return true;
}

Instruction::~Instruction() {
    std::vector<Argument *>::const_iterator it;
    for (it = args.begin(); it != args.end(); ++it)
        delete (*it);
}

std::string Instruction::str_condition_code() const {
    //FIXME stub
    return "";
}
