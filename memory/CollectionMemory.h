#ifndef __COLLECTION_MEMORY_H__
#define __COLLECTION_MEMORY_H__

#include "armyconfig.h"
#include "Memory.h"

#include <vector>
#include <utility>

class ARMYCORE_EXPORT CollectionMemory: public Memory {
public:
    void add_memrange(addr_t start, addr_t end, Memory *engine);
    std::string read(addr_t addr, size_t bytes);
    void       write(addr_t addr, std::string data);

private:
    std::pair<std::pair<addr_t,size_t>, Memory *> get_engine(addr_t addr);
    typedef std::vector<std::pair<std::pair<addr_t,addr_t>, Memory *> > engine_list_t;
    engine_list_t engines;
};

#endif

