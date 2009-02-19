#ifndef ARMY_MEMORY_ENDIAN_MEMORY_H__
#define ARMY_MEMORY_ENDIAN_MEMORY_H__

#include "armyconfig.h"
#include "memory/DecoratorMemory.h"

class ARMYCORE_EXPORT EndianMemory : public DecoratorMemory {
public:
    enum Size {
        Word = 4,
        Halfword = 2,
        Byte = 1
    };

    enum Signedness {
        Signed,
        Unsigned
    };

    EndianMemory(Memory *engine, Endianness end);

    ARM_Word   read_value (addr_t addr, Size size = Word, Signedness signedness = Unsigned);
    void       write_value(addr_t addr, ARM_Word value, Size size = Word);

private:
    Endianness endianness_;
};

#endif
