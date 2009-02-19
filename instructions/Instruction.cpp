#include "instructions/Instruction.h"

namespace arm {

std::ostream& operator<<(std::ostream& o, const Instruction& insn)
{
    insn.fmt(o);
    return o;
}

Instruction::Instruction()
{
}

Instruction::~Instruction()
{
}

}
