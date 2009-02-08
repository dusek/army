#ifndef __COLLECTION_MEMORY_H__
#define __COLLECTION_MEMORY_H__

#include "armyconfig.h"
#include "Memory.h"

class ARMYCORE_EXPORT CollectionMemory: public Memory {
public:
    CollectionMemory();
    void add_memrange(addr_t start, addr_t end, Memory *engine);
    std::string read(addr_t addr, size_t bytes);
    void       write(addr_t addr, const std::string &data);
    virtual void alloc_protect(addr_t addr, std::size_t size, int protect);
    virtual int get_protect(addr_t addr);

    ~CollectionMemory();

private:
    class CollectionMemoryImpl;
    CollectionMemoryImpl *const d;
};

#endif

