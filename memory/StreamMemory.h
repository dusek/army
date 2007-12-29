#ifndef __HAVE_STREAM_MEMORY_H__
#define __HAVE_STREAM_MEMORY_H__

#include <iostream>

#include "Memory.h"

class StreamMemory: public Memory {
public:
	StreamMemory(std::iostream *stream): stream(stream){};
	std::string read (addr_t addr, size_t bytes);
	void   write(addr_t addr, std::string data);
private:
	std::iostream *stream;
};

#endif

