#include <cassert>
#include "instructions/util.h"

int get_bit_range_value(uint32_t word, int start, int end)
{
    assert(start>=0);
    assert(end>start);
    assert(31>=end);

    word &= (1 << end) - 1;
    word >>= start;
    return word;
}
