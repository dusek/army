#include "test/memory/EndianMemoryTest.h"

#include "memory/EndianMemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EndianMemoryTest);

void EndianMemoryTest::testRead() {
    mem->write(0, "\x01\x02\x03\x04");
    mem->write(4, "\xf1\xf2\xf3\xf4");

    //1. Little Endian
    EndianMemory *mem_le = new EndianMemory(mem, LittleEndian);

    //a. Unsigned
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000001, mem_le->read_value(0, EndianMemory::Byte));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000201, mem_le->read_value(0, EndianMemory::Halfword));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x04030201, mem_le->read_value(0, EndianMemory::Word));

    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x000000f1, mem_le->read_value(4, EndianMemory::Byte));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x0000f2f1, mem_le->read_value(4, EndianMemory::Halfword));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xf4f3f2f1, mem_le->read_value(4, EndianMemory::Word));

    //b. Signed
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000001, mem_le->read_value(0, EndianMemory::Byte    , EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000201, mem_le->read_value(0, EndianMemory::Halfword, EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x04030201, mem_le->read_value(0, EndianMemory::Word    , EndianMemory::Signed));

    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xfffffff1, mem_le->read_value(4, EndianMemory::Byte    , EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xfffff2f1, mem_le->read_value(4, EndianMemory::Halfword, EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xf4f3f2f1, mem_le->read_value(4, EndianMemory::Word    , EndianMemory::Signed));

    //2. Big Endian
    EndianMemory *mem_be = new EndianMemory(mem, BigEndian);
    //a. Unsigned
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000001, mem_be->read_value(0, EndianMemory::Byte));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000102, mem_be->read_value(0, EndianMemory::Halfword));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x01020304, mem_be->read_value(0, EndianMemory::Word));

    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x000000f1, mem_be->read_value(4, EndianMemory::Byte));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x0000f1f2, mem_be->read_value(4, EndianMemory::Halfword));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xf1f2f3f4, mem_be->read_value(4, EndianMemory::Word));

    //b. Signed
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000001, mem_be->read_value(0, EndianMemory::Byte    , EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x00000102, mem_be->read_value(0, EndianMemory::Halfword, EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0x01020304, mem_be->read_value(0, EndianMemory::Word    , EndianMemory::Signed));

    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xfffffff1, mem_be->read_value(4, EndianMemory::Byte    , EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xfffff1f2, mem_be->read_value(4, EndianMemory::Halfword, EndianMemory::Signed));
    CPPUNIT_ASSERT_EQUAL((ARM_Word)0xf1f2f3f4, mem_be->read_value(4, EndianMemory::Word    , EndianMemory::Signed));
}

void EndianMemoryTest::testWrite() {
#define MY_TEST_WRITE(memory,value,str,size)\
    mem->write(0, std::string("\xce\xce\xce\xce"));\
    memory->write_value(0, value, size);\
    CPPUNIT_ASSERT_EQUAL(std::string(str), mem->read(0, 4));

    EndianMemory *mem_le = new EndianMemory(mem, LittleEndian);

    MY_TEST_WRITE(mem_le, 0x04030201, "\x01\x02\x03\x04", EndianMemory::Word);
    MY_TEST_WRITE(mem_le, 0x04030201, "\x01\x02\xce\xce", EndianMemory::Halfword);
    MY_TEST_WRITE(mem_le, 0x04030201, "\x01\xce\xce\xce", EndianMemory::Byte);

    MY_TEST_WRITE(mem_le, 0xf4f3f2f1, "\xf1\xf2\xf3\xf4", EndianMemory::Word);
    MY_TEST_WRITE(mem_le, 0xf4f3f2f1, "\xf1\xf2\xce\xce", EndianMemory::Halfword);
    MY_TEST_WRITE(mem_le, 0xf4f3f2f1, "\xf1\xce\xce\xce", EndianMemory::Byte);



    EndianMemory *mem_be = new EndianMemory(mem, BigEndian);

    MY_TEST_WRITE(mem_be, 0x04030201, "\x04\x03\x02\x01", EndianMemory::Word);
    MY_TEST_WRITE(mem_be, 0x04030201, "\x02\x01\xce\xce", EndianMemory::Halfword);
    MY_TEST_WRITE(mem_be, 0x04030201, "\x01\xce\xce\xce", EndianMemory::Byte);

    MY_TEST_WRITE(mem_be, 0xf4f3f2f1, "\xf4\xf3\xf2\xf1", EndianMemory::Word);
    MY_TEST_WRITE(mem_be, 0xf4f3f2f1, "\xf2\xf1\xce\xce", EndianMemory::Halfword);
    MY_TEST_WRITE(mem_be, 0xf4f3f2f1, "\xf1\xce\xce\xce", EndianMemory::Byte);
}
