#ifndef ARMLIB_STRING_H_
#define ARMLIB_STRING_H_

#include <types.h>

uint32_t strlen(const char *str);
uint32_t strtou32(const char *str);

void logu32(int fd, uint32_t val);

#endif

