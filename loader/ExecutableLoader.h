#ifndef __EXECUTABLE_LOADER_H__
#define __EXECUTABLE_LOADER_H__

#include <istream>

#include "memory/Memory.h"

class ARMYCORE_EXPORT ExecutableLoader {
public:
    ExecutableLoader(){}
    /**
     * Checks whether binary contents in "executable" represent an executable
     * of the format the loader recognizes (i.e. ELFExecutableLoader checks
     * there is ELF's magic in first 4 bytes):
     *   if not, returns false and does nothing else
     *   if yes, returns true and does the following:
     * Creates a memory image of executable from stream "executable" and
     * stores it in memory "memory".
     *
     * Return value:
     *   Address of entry point in the executable image.
     */
	virtual bool load(std::istream &executable, Memory *memory, addr_t& addr) = 0;
    virtual ~ExecutableLoader(){};
};

#endif
