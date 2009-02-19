#include "memory/DecoratorMemory.h"

DecoratorMemory::DecoratorMemory(Memory *engine_)
:
engine(engine_)
{
}

void DecoratorMemory::write(addr_t addr, const std::string &data)
{
    engine->write(addr, data);
}

std::string DecoratorMemory::read(addr_t addr, size_t bytes)
{
    return engine->read(addr, bytes);
}

void DecoratorMemory::alloc_protect(addr_t addr, std::size_t size, int protect)
{
    engine->alloc_protect(addr, size, protect);
}

int DecoratorMemory::get_protect(addr_t addr) const
{
    return engine->get_protect(addr);
}