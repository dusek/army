#ifndef __DECORATOR_MEMORY_H__
#define __DECORATOR_MEMORY_H__

#include "armyconfig.h"
#include "Memory.h"

class ARMYCORE_EXPORT DecoratorMemory: public Memory {
public:
    ~DecoratorMemory() { delete engine; }
protected:
    Memory *engine;
};

#endif

