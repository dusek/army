project(army)

SET(BINDIR ../bin)
IF(WIN32)
    SET(BINDIR ${BINDIR}/win)
ENDIF(WIN32)
IF(APPLE)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        SET(BINDIR ${BINDIR}/mac-os-x)
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
ELSE(APPLE)
    SET(BINDIR ${BINDIR}/unix)
ENDIF(APPLE)

SET(BINDIR ${BINDIR}/${CMAKE_BUILD_TYPE})
SET(EXECUTABLE_OUTPUT_PATH ${BINDIR})
SET(LIBRARY_OUTPUT_PATH ${BINDIR})


INCLUDE_DIRECTORIES(.)

SET(CORE_SRCS
    memory/StreamMemory.cpp
    memory/PagedMemory.cpp
    memory/CollectionMemory.cpp
    loader/ELFLoader.cpp
    core/ProgramStatusRegister.cpp
    core/CPURegisters.cpp
    core/RuntimeException.cpp
    )
ADD_LIBRARY(armycore SHARED ${CORE_SRCS})
SET_TARGET_PROPERTIES(armycore PROPERTIES
    COMPILE_FLAGS "-Darmycore_EXPORTS -fvisibility=hidden -Wall"
    )


FIND_LIBRARY(CPPUNIT_LIBRARY cppunit)
ADD_EXECUTABLE(armytest
    test/Run.cpp
    test/memory/File.cpp
    test/memory/MemoryTest.cpp
    test/memory/StreamMemoryTest.cpp
    test/memory/PagedMemoryTest.cpp
    test/core/ProgramStatusRegisterTest.cpp
    test/core/CPURegistersTest.cpp
    )
TARGET_LINK_LIBRARIES(armytest armycore cppunit)
SET_TARGET_PROPERTIES(armytest PROPERTIES
    COMPILE_FLAGS "-Wall"
    )
