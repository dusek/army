#include <sstream>
#include <utility>
#include <algorithm>
#include <memory>

#include "test/loader/ELFLoaderTest.h"

#include "test/loader/TestExecutables.h"
#include "loader/ELFLoader.h"
#include "memory/VirtualMemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ELFLoaderTest);

void ELFLoaderTest::testGarbage()
{
    ELFLoader loader;
    addr_t load_addr;

    std::auto_ptr<Memory> mem(new VirtualMemory);
    std::stringstream s1("\0x7fELf");
    std::stringstream s2("\0x7fElF");
    std::stringstream s3("\0x7feLF");
    std::stringstream s4("\0x7eELF");

    
    CPPUNIT_ASSERT(!loader.load(s1, mem.get(), load_addr));
    CPPUNIT_ASSERT(!loader.load(s2, mem.get(), load_addr));
    CPPUNIT_ASSERT(!loader.load(s3, mem.get(), load_addr));
    CPPUNIT_ASSERT(!loader.load(s4, mem.get(), load_addr));
}

void ELFLoaderTest::testLoad()
{
    ELFLoader loader;
    addr_t entry_point;
    std::auto_ptr<Memory> mem(new VirtualMemory);
    ELFTestExecutable_Fibonacci fib;
    std::stringstream elf_contents(fib.contents);
    CPPUNIT_ASSERT(loader.load(elf_contents, mem.get(), entry_point));
    CPPUNIT_ASSERT_EQUAL(fib.entry_point, entry_point);
    check_loaded_file(fib, *mem);

    entry_point = 0;
    std::auto_ptr<Memory> mem2(new VirtualMemory);
    ELFTestExecutable_Prec prec;
    std::stringstream elf_contents2(prec.contents);
    CPPUNIT_ASSERT(loader.load(elf_contents2, mem2.get(), entry_point));
    CPPUNIT_ASSERT_EQUAL(prec.entry_point, entry_point);
    check_loaded_file(prec, *mem2);
}