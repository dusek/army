#ifndef __HAVE_STREAM_MEMORY_H__
#define __HAVE_STREAM_MEMORY_H__

#include <iostream>

#include "armyconfig.h"
#include "Memory.h"

class ARMYCORE_EXPORT StreamMemory: public Memory {
public:
    StreamMemory(std::iostream *stream): stream(stream){};
    std::string read (addr_t addr, size_t bytes);
    void   write(addr_t addr, const std::string &data);
    virtual void alloc_protect(addr_t addr, std::size_t size, int protect);
    virtual int get_protect(addr_t addr) const;
    virtual ~StreamMemory();
private:
    std::iostream *stream;
};

#endif

