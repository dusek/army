#ifndef TEST_MEMORY_UTIL_H
#define TEST_MEMORY_UTIL_H

#include <fstream>

class File {
public:
    static std::iostream *get_tmp_file();
};

#endif
