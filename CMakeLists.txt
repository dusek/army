cmake_minimum_required(VERSION 2.6)
project(army)

if(CMAKE_COMPILER_IS_GNUCC)
      add_definitions(-Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)

SET(BINDIR ${CMAKE_BUILD_TYPE})
SET(EXECUTABLE_OUTPUT_PATH ${BINDIR})
SET(LIBRARY_OUTPUT_PATH ${BINDIR})


INCLUDE_DIRECTORIES(.)

SET(CORE_SRCS
    memory/StreamMemory.cpp
    memory/PagedMemory.cpp
    memory/CollectionMemory.cpp
    memory/DecoratorMemory.cpp
    memory/EndianMemory.cpp
    memory/VirtualMemory.cpp
    loader/ELFLoader.cpp
    loader/PELoader.cpp
    platform/Time.cpp
    core/CPU.cpp
    core/ProgramStatusRegister.cpp
    core/CPURegisters.cpp
    core/RuntimeException.cpp
    instructions/cond.cpp
    instructions/Arg.cpp
    instructions/RegArg.cpp
    instructions/ImmedArg.cpp
    instructions/ShOpShiftArg.cpp
    instructions/ShOpLSLArg.cpp
    instructions/ShOpSRArg.cpp
    instructions/ShOpRORArg.cpp
    instructions/ShOpRRXArg.cpp
    instructions/Instruction.cpp
    instructions/DPInstruction.cpp
    instructions/Subtraction.cpp
    instructions/Addition.cpp
    instructions/AndInstruction.cpp
    instructions/EorInstruction.cpp
    instructions/OrrInstruction.cpp
    instructions/BranchInstruction.cpp
    instructions/MoveInstruction.cpp
    instructions/CondInstruction.cpp
    instructions/LoadStoreInstruction.cpp
    instructions/LoadStoreMultipleInstruction.cpp
    instructions/SWIInstruction.cpp
    instructions/MultiplyInstruction.cpp
    instructions/CLZInstruction.cpp
    instructions/InsnDecoder.cpp
    instructions/ARMInsnDecoder.cpp
    instructions/CachingInsnDecoder.cpp
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
    test/memory/VirtualMemoryTest.cpp
    test/memory/EndianMemoryTest.cpp
    test/core/ProgramStatusRegisterTest.cpp
    test/core/CPURegistersTest.cpp
    test/loader/TestExecutables.cpp
    test/loader/ELFLoaderTest.cpp
    )
TARGET_LINK_LIBRARIES(armytest armycore ${CPPUNIT_LIBRARY})
SET_TARGET_PROPERTIES(armytest PROPERTIES
    COMPILE_FLAGS "-Wall"
    )

ADD_EXECUTABLE(armyexec
    exec/main.cpp
    )
TARGET_LINK_LIBRARIES(armyexec armycore)
