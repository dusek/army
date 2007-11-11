#include "instructions/RegisterArgument.h"

#include <sstream>

static const ARM_Register reg_map[16] = {
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
};

void RegisterArgument::decode(uint32_t insn_word, int start, int end)
{
    reg_ = reg_map[get_bit_range_value(insn_word, start, end)];
}

uint32_t RegisterArgument::read(CPU &cpu) const
{
    return cpu.get_reg(reg_);
}

void RegisterArgument::store(CPU &cpu, uint32_t value) const
{
    cpu.set_reg(reg_, value);
}

std::string RegisterArgument::to_string() const
{
    std::ostringstream ret;
    ret << "R";
    ret << (int)reg_;
    return ret.str();
}
