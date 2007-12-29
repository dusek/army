#include "ImmediateArgument.h"

#include <cassert>
#include <sstream>
#include "instructions/util.h"

void ImmediateArgument::decode(uint32_t insn_word, int start, int end) {
    value_ = get_bit_range_value(insn_word, start, end);
}

uint32_t ImmediateArgument::read(CPU &cpu) const {
    return value_;
}

void ImmediateArgument::store(CPU &cpu, uint32_t value) const {
    assert(false);//can't write to an immediate value
}

std::string ImmediateArgument::to_string() const {
    std::ostringstream oss;
    oss << "#" << value_;
    return oss.str();
}
