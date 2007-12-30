#ifndef __DECORATOR_MEMORY_H__
#define __DECORATOR_MEMORY_H__

#include "Memory.h"

class DecoratorMemory: public Memory {
public:
	~DecoratorMemory() { delete engine; }
protected:
	Memory *engine;
};

#endif

