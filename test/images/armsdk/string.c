#include <string.h>

uint32_t strlen(const char *str)
{
    const char *const str_begin = str;
    while (*str != '\0')
        ++str;

    return str - str_begin;
}

uint32_t strtou32(const char *str)
{
    int val = 0;
    while (*str) {
        val *= 10;
        val += *str - '0';
        ++str;
    }
    return val;
} 

void logu32(int fd, uint32_t val)
{
    char c = '0';
    uint32_t level = 1;
    if (val == 0)
        write(fd, &c, 1);
    else {
        uint32_t old_level = level;
        while (level <= val) {
            old_level = level;
            level += old_level << 2;
            level += level;
        }
        level = old_level;
        while (level > 0) {
            c = (val / level) + '0';
            write(fd, &c, 1);
            val %= level;
            level /= 10;
        }
    }
    write(fd, "\n", 1);
}
