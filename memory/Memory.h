#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <string>
#include "armyconfig.h"
#include "platform/pstdint.h"

typedef uint32_t addr_t;

class ARMYCORE_EXPORT Memory
{
public:
    /**
     * This default constructor does nothing. It must be declared because it's
     * needed in subclasses' default constructors and since fixing C4512 below
     * prevents compiler from generating a default Memory constructor itself.
     */
    Memory(){};

    virtual std::string read (addr_t addr, size_t bytes) = 0;
    virtual void   write(addr_t addr, std::string data) = 0;

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
