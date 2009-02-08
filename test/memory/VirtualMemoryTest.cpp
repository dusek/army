#include "test/memory/VirtualMemoryTest.h"

#include "memory/VirtualMemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VirtualMemoryTest);

void VirtualMemoryTest::setUp()
{
    MemoryTest::setUp();
    mem = new VirtualMemory();
}

void VirtualMemoryTest::testAll()
{
    mem->write(0x00008002, "AAA");//800->000
    mem->write(0x00003000, "BBB");//300->100
    mem->write(0x00001000, "Hello");//100->200
    mem->write(0x00005000, "CCC");//500->300
    mem->write(0x00007000, "DDD");//600->400
    mem->write(0x00000000, "BBB");//000->500

    mem->write(0x00020000, "World");//2000->600
    mem->write(0x00000ffe, "1234");//nothing

    //Read across a page boundary (0x0ff -> 0x5ff; 0x100 -> 0x200; 0x101 -> 0x201)
    CPPUNIT_ASSERT_EQUAL(mem->read(0x00000fff, 3), std::string("234"));
}
