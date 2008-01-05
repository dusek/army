#include "test/memory/StreamMemoryTest.h"

#include "test/memory/File.h"
#include "memory/StreamMemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(StreamMemoryTest);

enum {
    Count = 1000
};

void StreamMemoryTest::setUp()
{
    int i;
    MemoryTest::setUp();
    std::iostream *f = File::get_tmp_file();
    for (i = 0; i < Count; i++)
        f->write("abcdefghijklmnopqrstuvwxyz", 26);
    delete mem;
    mem = new StreamMemory(f);
}

void StreamMemoryTest::testRead()
{
    addr_t addr;
    int i;
    std::string read;
    std::string shifted("klmnopqrstuvwxyzabcdefghij");
    for (i = 0; i < Count-1; i++) {
        addr = i*26 + 10;
        read = mem->read(addr, 26);
        CPPUNIT_ASSERT_EQUAL(shifted, read);
    }
}