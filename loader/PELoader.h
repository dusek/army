#ifndef ARMY_CORE_PE_LOADER_H__
#define ARMY_CORE_PE_LOADER_H__

#include "ExecutableLoader.h"

class ARMYCORE_EXPORT PELoader: public ExecutableLoader {
public:
	PELoader() {};
	virtual bool load(std::istream &executable, Memory *memory, addr_t& addr);
    ~PELoader() {};
};

#endif
