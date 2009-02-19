#include <cassert>

#include "instructions/ImmedArg.h"

ImmedArg::ImmedArg(ARM_Word immed)
: immed_(immed)
{
}

ImmedArg::~ImmedArg()
{
}

ARM_Word ImmedArg::load(CPURegisters &, bool) const
{
    return immed_;
}

ARM_Word ImmedArg::val() const
{
    return immed_;
}

void ImmedArg::fmt(std::ostream &o) const
{
    o << '#' << immed_;
}

ImmedArg *ImmedArg_decode(ARM_Word word, std::size_t bit_pos, std::size_t size, bool to_double)
{
    return new ImmedArg(ImmedArg_ValFromWord(word, bit_pos, size, to_double));
}

ARM_Word ImmedArg_ValFromWord(ARM_Word word, std::size_t bit_pos, std::size_t size, bool to_double)
{
    assert(bit_pos + size <= 32);
    assert(bit_pos >= 0);
    
    word >>= bit_pos;
    ARM_Word mask = ~(0xffffffff << size);
    word &= mask;
    if (to_double)
        word <<= 1;

    return word;
}