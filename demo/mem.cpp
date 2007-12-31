#include <iostream>
#include <fstream>
#include <cassert>

#include "memory/StreamMemory.h"
#include "memory/PagedMemory.h"
#include "memory/CollectionMemory.h"

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Exactly one argument required, " << argc << " supplied." << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream file;
    file.open(argv[1], std::fstream::binary | std::fstream::in |
        std::fstream::out | std::fstream::trunc);
    Memory *mem1 = new PagedMemory(new StreamMemory(&file), 8);

    mem1->write(0x00000802, "AAA");//800->000
    mem1->write(0x00000300, "BBB");//300->100
    mem1->write(0x00000100, "Hello");//100->200
    mem1->write(0x00000500, "CCC");//500->300
    mem1->write(0x00000700, "DDD");//600->400
    mem1->write(0x00000000, "BBB");//000->500

    mem1->write(0x00002000, "World");//2000->600
    mem1->write(0x000000fe, "1234");//nothing

    std::fstream second_file;
    second_file.open(argv[2], std::fstream::binary | std::fstream::in |
        std::fstream::out | std::fstream::trunc);
    Memory *mem2 = new StreamMemory(&second_file);
    CollectionMemory *mem = new CollectionMemory();
    mem->add_memrange(0x10000000, 0xb0000000, mem1);
    mem->add_memrange(0xb0000000, 0xffffffff, mem2);

    std::cout << "Whether read test is OK: " << std::boolalpha << (mem1->read(0x000000ff, 3) == std::string("234")) << std::endl;

    std::string transfer = mem->read(0x100000fe, 3);
    mem->write(0xb0000011, transfer);
    assert(mem2->read(0x00000011, 3) == transfer);
    assert( mem->read(0xb0000011, 3) == transfer);

    delete mem1;
    delete mem2;
    delete mem;
    file.close();
    second_file.close();
    return EXIT_SUCCESS;
}

