#ifndef __ELF_LOADER_H__
#define __ELF_LOADER_H__

#include "ExecutableLoader.h"

class ARMYCORE_EXPORT ELFLoader: public ExecutableLoader {
public:
	ELFLoader() {};
	virtual bool load(std::istream &executable, Memory *memory, addr_t& addr);
    ~ELFLoader() {};
};

#endif
