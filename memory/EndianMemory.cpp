#include <map>
#include <algorithm>
#include <cassert>

#include "memory/EndianMemory.h"

/**
 * Returns "data", supposed to be in memory endianness, converted to platform
 * endianness (platform endianness = endianness of CPU on which this ARM interpreter
 * runs.
 */
static void endian_swap(std::string& data, Endianness memory_endianness)
{
    if (memory_endianness != runtime_platform_endianness) {
        std::reverse(data.begin(), data.end());
    }
}

EndianMemory::EndianMemory(Memory *engine, Endianness end)
:
DecoratorMemory(engine),
endianness_(end)
{
    this->engine = engine;
}

ARM_Word EndianMemory::read_value(addr_t addr, EndianMemory::Size size, EndianMemory::Signedness signedness)
{
    std::string data = engine->read(addr, size);
    if (endianness_ == BigEndian)
        std::reverse(data.begin(), data.end());
    ARM_Word ret = 0;
    for (int i = 0; i < size; i++)
        ret |= ((unsigned char)data[i]) << 8*i;
    
    //sign-extend the value, if needed
    if (signedness == Signed) {
        bool perform = true;
        ARM_Word signmask = 0;
        ARM_Word extendbits = 0;
        switch (size) {
        case Word:
            perform = false;
            break;
        case Halfword:
            signmask   = 0x00008000;
            extendbits = 0xffff0000;
            break;
        case Byte:
            signmask   = 0x00000080;
            extendbits = 0xffffff00;
            break;
        default:
            assert(!"Unknown value size");
            perform = false;
            break;
        }
        if (perform) {
            if (ret & signmask) {
                ret |= extendbits;
            }
        }
    }

    return ret;
}

void EndianMemory::write_value(addr_t addr, ARM_Word value, EndianMemory::Size size)
{
    std::string data((char *) &value, sizeof(ARM_Word));
    endian_swap(data, endianness_);
    int start = 0;
    if (endianness_ == BigEndian)
        start = sizeof(ARM_Word) - size;
    data = data.substr(start, size);
    engine->write(addr, data);
}