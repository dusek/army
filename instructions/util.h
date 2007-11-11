#ifndef __UTIL_H__
#define __UTIL_H__

#include "platform/pstdint.h"

/**
 * Gets unsigned value of integer represented by bits (start ... end-1)
 * of word "word".
 */
int get_bit_range_value(uint32_t word, int start, int end);

#endif
