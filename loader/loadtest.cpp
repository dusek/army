#include <fstream>

#include "StreamMemory.h"
#include "PagedMemory.h"
#include "ELFLoader.h"

int main (int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Exactly 2 arguments needed" << std::endl;
		return 1;
	}

	std::fstream storage(argv[1], std::ios_base::in | std::ios_base::out |
		std::ios_base::binary | std::ios_base::trunc);

	std::fstream exe(argv[2], std::ios_base::in | std::ios_base::binary);

	Memory *mem = new PagedMemory(new StreamMemory(&storage));
	ExecutableLoader *loader = new ELFLoader(&exe);
	loader->load(mem);

	delete mem;
	delete loader;
	storage.close();
	exe.close();
	
	return 0;
}