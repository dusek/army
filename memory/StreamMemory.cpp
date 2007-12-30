#include <string>

#include "StreamMemory.h"

std::string StreamMemory::read(addr_t addr, size_t bytes) {
	this->stream->seekg(addr);
	char *buffer = new char[bytes];
	this->stream->read(buffer, bytes);
	std::string ret(buffer,bytes);
	delete[] buffer;
	return ret;
}

void StreamMemory::write(addr_t addr, std::string data) {
	this->stream->seekp(addr);
	this->stream->write(data.c_str(), data.length());
}

