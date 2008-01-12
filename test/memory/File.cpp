#include <fstream>

#include "test/memory/File.h"

std::iostream *
File::get_tmp_file()
{
    std::fstream *ret;
    
    ret = new std::fstream("a390th2gidh0938h9u8ahg8hgjs",
        std::ios_base::trunc | std::ios_base::in | std::ios_base::out | std::ios_base::binary);

    return ret;
}
