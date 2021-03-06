#ifndef __PAGED_MEMORY_H__
#define __PAGED_MEMORY_H__

#include "armyconfig.h"
#include "DecoratorMemory.h"

class ARMYCORE_EXPORT PagedMemory: public DecoratorMemory {
public:
    PagedMemory(Memory *engine, size_t page_size_exp = 12);
    virtual std::string read (addr_t addr, size_t bytes);
    virtual void   write(addr_t addr, const std::string &data);
    virtual void alloc_protect(addr_t addr, std::size_t, int protect);
    virtual int get_protect(addr_t addr) const;

protected:
    /**
     * Returns physical real address corresponding to the supplied
     * virtual address.
     *
     * virtual_addr - virtual address
     * create_on_demand - if set, and if there is no page mapping the
     *                    requested virtual address, create new page
     *                    for this virtual address and return it;
     *                    if not set, returns 0 as address.
     *
     * Return value:
     * std::pair<addr_t real_address, int left>
     * real_address - the requested real address
     * left - how many bytes are left on this page starting at real_address
     */
    virtual std::pair<addr_t,size_t> virtual_to_physical(addr_t virtual_addr,
                                                bool create_on_demand = true);

private:
    class PagedMemoryImpl;
    PagedMemoryImpl *const d;
};

#endif

