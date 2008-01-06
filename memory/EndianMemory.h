#ifndef __ENDIAN_MEMORY_H__
#define __ENDIAN_MEMORY_H__

#include "armyconfig.h"
#include "memory/DecoratorMemory.h"

class ARMYCORE_EXPORT EndianMemory: public DecoratorMemory {
public:
    enum Endianness {
        LittleEndian,
        BigEndian
    };
    EndianMemory(Memory *engine, Endianness end);
    std::string read(addr_t addr, size_t bytes);
    void       write(addr_t addr, const std::string &data);

    int   read(addr_t addr, size_t bytes = 4, bool signed_ = false);
    void write(addr_t addr, int value, size_t bytes = 4, bool signed_ = false);

private:
    Endianness endianness;
};

#endif
