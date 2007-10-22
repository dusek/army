#ifndef __EXECUTABLE_LOADER_H__
#define __EXECUTABLE_LOADER_H__

#include <iostream>

#include "Memory.h"

class ExecutableLoader {
public:
	ExecutableLoader(std::iostream *executable): executable(executable){}
	/**
	 * Create a memory image of executable from stream "executable" and
	 * stores it in memory "memory"
	 *
	 * Return value:
	 *   Address of entry point in the executable image.
	 */
	virtual addr_t load(Memory *memory) = 0;
	virtual ~ExecutableLoader(){};

protected:
	std::iostream *executable;
};

#endif
