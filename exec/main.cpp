#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>

#include "memory/VirtualMemory.h"
#include "memory/EndianMemory.h"

#include "loader/ELFLoader.h"
#include "loader/PELoader.h"

#include "core/CPU.h"

#include "platform/Time.h"

void usage(std::ostream& o)
{
    o << "Usage: armyexec 1|0 image_file [guest_arg1 [guest_arg2[ ... ]]" << std::endl;
}

int main(int argc, const char **argv)
{
    if (argc < 3) {
        usage(std::cerr);
        return EXIT_FAILURE;
    }

    const char *image_flnm = argv[2];
    
    bool log = false;
    bool log_dumpregs = false;
    if (argv[1][0] >= '1')
        log = true;
    if (argv[1][0] >= '2')
        log_dumpregs = true;

    std::fstream image(image_flnm, std::ios_base::binary | std::ios_base::in);
    if (image.fail()) {
        std::cerr << "Could not open image file " << image_flnm << std::endl;
        return EXIT_FAILURE;
    }

    EndianMemory mem(new VirtualMemory, LittleEndian);

    std::auto_ptr<ExecutableLoader> elfloader(new ELFLoader);
    std::auto_ptr<ExecutableLoader> peloader(new PELoader);

    addr_t entry_point;

    if (!elfloader->load(image, &mem, entry_point) && !peloader->load(image, &mem, entry_point)) {
        std::cerr << "Image in " << image_flnm << "is neither ELF nor PE format, or contains some error." << std::endl;
        return EXIT_FAILURE;
    }

    CPU cpu(mem, log, log_dumpregs, argc - 2, argv + 2);
    std::cout << "Loaded image, starting execution" << std::endl;
    Time time_start = Time::now();
    cpu.run(entry_point);
    Time time_end = Time::now();
    Time duration = time_end - time_start;

    std::cout << "Execution ended." << std::endl;
    std::cout << "Executed " << cpu.pc_counter() << " instructions." << std::endl;
    std::cout.setf(std::ios_base::fixed);
    std::cout.precision(2);
    std::cout << "Execution took " << duration << " (" << ((double)cpu.pc_counter())/((double)duration.time()) << " \"MHz\")" << std::endl;

    return EXIT_SUCCESS;
}
