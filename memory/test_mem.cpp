#include <iostream>
#include <fstream>
#include "StreamMemory.h"
#include "PagedMemory.h"

int main (int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Exactly one argument required, " << argc << " supplied." << std::endl;
		return EXIT_FAILURE;
	}

	std::fstream file;
	file.open(argv[1], std::fstream::binary | std::fstream::in |
		std::fstream::out | std::fstream::trunc);
	Memory *mem = new PagedMemory(new StreamMemory(&file), 8);

	mem->write(0x00000802, "AAA");//800->000
	mem->write(0x00000300, "BBB");//300->100
	mem->write(0x00000100, "Hello");//100->200
	mem->write(0x00000500, "CCC");//500->300
	mem->write(0x00000700, "DDD");//600->400
	mem->write(0x00000000, "BBB");//000->500

	mem->write(0x00002000, "World");//2000->600
	mem->write(0x000000fe, "1234");//nothing

	std::cout << "Whether read test is OK: " << std::boolalpha << (mem->read(0x000000ff, 3) == std::string("234")) << std::endl;

	delete mem;
	file.close();
	return EXIT_SUCCESS;
}

