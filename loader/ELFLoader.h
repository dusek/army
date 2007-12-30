#ifndef __ELF_LOADER_H__
#define __ELF_LOADER_H__

#include "ExecutableLoader.h"

class ELFLoader: public ExecutableLoader {
public:
	ELFLoader(std::iostream *executable);
	addr_t load(Memory *memory);
	~ELFLoader() {};
};

#endif
