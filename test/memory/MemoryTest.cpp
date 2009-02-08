#include "test/memory/MemoryTest.h"

#include "test/memory/File.h"
#include "memory/StreamMemory.h"

enum {
    Count = 1000
};

void MemoryTest::setUp()
{
    mem = new StreamMemory(File::get_tmp_file());
}

void MemoryTest::tearDown()
{
    delete mem;
}

void MemoryTest::testBasic()
{
    addr_t addr;
    int i;
    int gap = 3;
    int init_offset = 5;

    std::string read;
    std::string written("s938jener85skwywyr87vhsmxlxosd78fu4");

    //Fill the memory with some random data (the "written" string)
    for (i = 0; i < Count; i++) {
        addr = i*(written.length() + gap) + init_offset;
        mem->write(addr, written);
    }

    //Read the just written memory in various ways and compare with expected parts
    //of "written" string
    std::string slice = written.substr(10, 5);
    for (i = 0; i < Count; i++) {
        addr = i*(written.length() + gap) + init_offset;
        read = mem->read(addr + 10, 5);
        CPPUNIT_ASSERT_EQUAL(slice, read);
        read = mem->read(addr, written.length());
        CPPUNIT_ASSERT_EQUAL(written, read);
    }

    // Read of size 0 always returns empty string
    std::string empty("");
    for (addr = 0; addr < init_offset + (Count - 1)*(written.length() + gap) + written.length(); addr++)
        CPPUNIT_ASSERT_EQUAL(empty, mem->read(addr, 0));
}
