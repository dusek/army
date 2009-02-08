#ifndef __DECORATOR_MEMORY_H__
#define __DECORATOR_MEMORY_H__

#include "armyconfig.h"
#include "Memory.h"

class ARMYCORE_EXPORT DecoratorMemory: public Memory {
public:
    ~DecoratorMemory() { delete engine; }
    virtual std::string read (addr_t addr, size_t bytes) = 0;
    virtual void   write(addr_t addr, const std::string &data) = 0;
    virtual void alloc_protect(addr_t addr, std::size_t size, int protect) = 0;
    virtual int get_protect(addr_t addr) = 0;
protected:
    Memory *engine;
};

#endif

