#ifndef ARMY_TEST_TESTEXECUTABLES_H_
#define ARMY_TEST_TESTEXECUTABLES_H_

#include <string>
#include <utility>
#include <vector>
#include "memory/Memory.h"

/**
 * Creates a testing ELF executable (the same as for an older fibonacci example)
 */

struct Segment {
    Segment(addr_t vm_start_, addr_t fl_start_, std::size_t size_);

    addr_t vm_start; // start in virtual memory
    addr_t fl_start; // start in file
    std::size_t size;
};

struct ELFTestExecutable {

public:

protected:
    ELFTestExecutable();

public:
    std::string contents;
    std::vector<Segment> segments;
    addr_t entry_point;
};

struct ELFTestExecutable_Fibonacci : public ELFTestExecutable
{
public:
    ELFTestExecutable_Fibonacci();
};

struct ELFTestExecutable_Prec : public ELFTestExecutable
{
public:
    ELFTestExecutable_Prec();
};

void check_loaded_file(const ELFTestExecutable& exe, Memory& mem);

#endif
