#include "CollectionMemory.h"
#include <cassert>

void CollectionMemory::add_memrange(addr_t start, addr_t end, Memory *engine) {
    std::pair<addr_t,addr_t> memrange(start,end);
    engines.push_back(std::pair<std::pair<addr_t,addr_t>, Memory *>(memrange, engine));
}

std::pair<std::pair<addr_t,size_t>, Memory *> CollectionMemory::get_engine(addr_t addr) {
    Memory *engine = 0;
    addr_t offset = 0;
    size_t left = 0;
    engine_list_t::const_iterator it;
    for (it = engines.begin(); it != engines.end(); ++it) {
        addr_t start = it->first.first;
        addr_t end = it->first.second;
        if ((start <= addr) && (addr < end)) {
            engine = it->second;
            offset = start;
            left = end - addr;
        }
    }
    std::pair<std::pair<addr_t,size_t>, Memory *> ret(std::pair<addr_t,size_t>(offset,left), engine);
    return ret;
}

std::string CollectionMemory::read(addr_t addr, size_t bytes) {
    std::string ret;

    std::pair<std::pair<addr_t,size_t>, Memory *> target_info = get_engine(addr);
    addr_t offset = target_info.first.first;
    addr_t left = target_info.first.second;

    assert(left > 0);
    assert(left >= bytes);
    addr -= offset;
    Memory *engine = target_info.second;
    return engine->read(addr, bytes);
}

void CollectionMemory::write(addr_t addr, std::string data) {
    std::string ret;

    std::pair<std::pair<addr_t,size_t>, Memory *> target_info = get_engine(addr);
    addr_t offset = target_info.first.first;
    addr_t left = target_info.first.second;

    assert(left > 0);
    assert(left >= data.length());
    addr -= offset;
    Memory *engine = target_info.second;
    engine->write(addr, data);
}
