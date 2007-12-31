#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <string>
#include "armyconfig.h"
#include "platform/pstdint.h"

typedef uint32_t addr_t;

class ARMYCORE_EXPORT Memory
{
public:
    virtual std::string read (addr_t addr, size_t bytes) = 0;
    virtual void   write(addr_t addr, std::string data) = 0;

    virtual ~Memory(void){};
};

#endif
