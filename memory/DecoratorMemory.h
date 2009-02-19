#ifndef __DECORATOR_MEMORY_H__
#define __DECORATOR_MEMORY_H__

#include "armyconfig.h"
#include "Memory.h"

class ARMYCORE_EXPORT DecoratorMemory: public Memory {
public:
    ~DecoratorMemory() { delete engine; }
    virtual std::string read (addr_t addr, size_t bytes);
    virtual void   write(addr_t addr, const std::string &data);
    virtual void alloc_protect(addr_t addr, std::size_t size, int protect);
    virtual int get_protect(addr_t addr) const;
protected:
    DecoratorMemory(Memory *engine);
    Memory *engine;
};

#endif

