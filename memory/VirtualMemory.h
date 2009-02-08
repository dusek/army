#ifndef ARMY_CORE_VIRTUALMEMORY_H_
#define ARMY_CORE_VIRTUALMEMORY_H_

#include "memory/Memory.h"

/**
 * Memory backed by pages allocated directly from OS
 * (i.e. using Virtual{Alloc,Free} on WinNT, m{map,unmap} on UNIX
 */
class ARMYCORE_EXPORT VirtualMemory : public Memory
{
public:
    VirtualMemory();
    virtual ~VirtualMemory();

    virtual std::string read(addr_t addr, std::size_t bytes);
    virtual void write(addr_t addr, const std::string& data);

    virtual void alloc_protect(addr_t addr, std::size_t size, int protect);
    virtual int get_protect(addr_t addr);

private:
    struct VMPrivate;
    VMPrivate *const pimpl_;
};

#endif
