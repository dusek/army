#ifndef MEMORY_TEST_H
#define MEMORY_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class Memory;

/**
 * Basic class for memory test
 * it includes checks for properties that consist basic semantics
 * of any memory implementation (except for special cases like IO device).
 * This includes the following:
 *   * read of size 0 returns empty string ("")
 *   * write to the memory; on subsequent read, the contents have not
 *     changed from the value just written
 *
 * Following documentation shows hot to subclass MemoryTest properly. It
 * assumes that the memory class you are testing is named SpecialMemory.
 * Also look at "MemoryTest::setUp()" for further essential information.
 *
 * 1. See setUp() and tearDown() documentation below to learn additional essential
 *    documentation on how to subclass MemoryTest properly. That documentation
 *    assumes that the Memory class you are testing is named SpecialMemory.
 *
 * 2. You declare your test suite as usual, just use CPPUNIT_TEST_SUB_SUITE
 *    instead of CPPUNIT_TEST_SUITE, specifying from which test you inherit, and
 *    don't include the tests already contained in the parent test fixture.
 *
 *    Example:
 *
 *      \code
 *      CPPUNIT_TEST_SUB_SUITE(SpecialMemoryTest, MemoryTest);
 *      CPPUNIT_TEST(testSpecial1());
 *      CPPUNIT_TEST(testSpecial2());
 *      //...
 *      CPPUNIT_TEST(testSpecialN());
 *      CPPUNIT_TEST_SUITE_END();
 *      \endcode
 *
 * 3. Don't forget to register the test suite by adding
 *
 *    \code
 *    CPPUNIT_TEST_SUITE_REGISTRATION(SpecialMemoryTest);
 *    \endcode
 *
 *    to the implementation file.
 */
class MemoryTest: public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(MemoryTest);
    CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST_SUITE_END_ABSTRACT();

public:
    /**
     * Override setUp() to instantiate your memory.
     * As a very first thing, chain to the parent (call MemoryTest::setUp()).
     * Then, "mem" will contain a storage memory, on top of which you can create
     * your memory. Do something like this:
     *
     *\code
     *  void SpecialMemoryTest::setUp() {
     *      MemoryTest::setUp();
     *      SpecialMemory *spec_mem = new SpecialMemory(mem, special_memory_args...);
     *      // Do SpecialMemory-specific set-up here
     *      // ...
     *      // Finally:
     *      mem = spec_mem;
     *  }
     *\endcode
     *
     * If you don't have to do any specific set-up steps, you can just do:
     *\code
     *  mem = new SpecialMemory(mem, special_memory_args...);
     * 
     * You can also declare spec_mem as a private variable, in order to not
     * have to do static_cast<SpecialMemory> on any test that requires methods
     * specific to your memory. The choice is your, possibilities limitless.
     */
    void setUp();

    /**
     * MemoryTest::tearDown just calls "delete mem". If that is what you want,
     * you don't have to override this method. Note that "delete mem" does a lot - 
     * it destroys all memory in the hierarchy below "mem". For StreamMemory,
     * which is usually at the bottom of this hierarchy, destructor closes the
     * file by which that memory was backed.
     *
     * If you override the method, either do "delete mem" yourself, or chain
     * into the parent by "MemoryTest::tearDown()" as a last expression.
     */
    void tearDown();

    /**
     * This is the basic test described above. You should not have to deal with it
     * at all.
     */
    //TODO see if I can make it private:
    void testBasic();

protected:
    Memory *mem;
};

#endif
