#include "test/memory/PagedMemoryTest.h"

#include "memory/PagedMemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PagedMemoryTest);

void PagedMemoryTest::setUp()
{
    MemoryTest::setUp();
    mem = new PagedMemory(mem, 8);
}

void PagedMemoryTest::testAll()
{
    mem->write(0x00000802, "AAA");//800->000
    mem->write(0x00000300, "BBB");//300->100
    mem->write(0x00000100, "Hello");//100->200
    mem->write(0x00000500, "CCC");//500->300
    mem->write(0x00000700, "DDD");//600->400
    mem->write(0x00000000, "BBB");//000->500

    mem->write(0x00002000, "World");//2000->600
    mem->write(0x000000fe, "1234");//nothing

    //Read across a page boundary (0x0ff -> 0x5ff; 0x100 -> 0x200; 0x101 -> 0x201)
    CPPUNIT_ASSERT_EQUAL(mem->read(0x000000ff, 3), std::string("234"));
}