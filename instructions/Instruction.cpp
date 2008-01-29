#include "instructions/Instruction.h"

Instruction::~Instruction()
{
}

std::ostream &operator<<(std::ostream &o, const Instruction &instruction)
{
    instruction.print(o);
    return o;
}