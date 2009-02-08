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

void StreamMemory::write(addr_t addr, const std::string &data) {
    this->stream->seekp(addr);
    this->stream->write(data.c_str(), data.length());
}

void StreamMemory::alloc_protect(addr_t /*addr*/, std::size_t /*size*/, int /*protect*/)
{
    //no-op here
}

StreamMemory::~StreamMemory() {
    delete stream;
}

int StreamMemory::get_protect(addr_t /*addr*/)
{
    return Memory::Read | Memory::Write | Memory::Execute;
}