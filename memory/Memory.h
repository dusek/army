#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <string>
#include "armyconfig.h"
#include "platform/pstdint.h"

class ARMYCORE_EXPORT Memory
{
public:
    enum Protection {
        None = 0x0,
        Read = 0x1,
        Write = 0x2,
        Execute = 0x4
    };

    /**
     * This default constructor does nothing. It must be declared because it's
     * needed in subclasses' default constructors and since fixing C4512 below
     * prevents compiler from generating a default Memory constructor itself.
     */
    Memory(){};

    virtual std::string read (addr_t addr, size_t bytes) = 0;
    virtual void   write(addr_t addr, const std::string &data) = 0;

    /*
    ** Protects all pages in range [addr, addr + size) with "protect"
    */
    virtual void alloc_protect(addr_t addr, std::size_t size, int protect) = 0;
    
    /* Return value: protection of unique page at virtual address "addr",
    ** or -1 if this page does not exist.
    */
    virtual int get_protect(addr_t addr) = 0;

    virtual ~Memory(void){};

private:
    /* This is to fix C4512 in some classes. It's correct since copying
     * memory with assignment operator is not the way we want it;
     * if we want it, then by a specialized function.
     * Furthermore, this operator should not really be needed since
     * Memory will be used with pointers mostly.
     *
     * While we are at it, we also declare copy constructor as private
     * for the same reason. (The warning was only for assignment operator).
     */

    Memory(const Memory &);
    Memory &operator=(const Memory &);
};

#endif
