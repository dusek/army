include_directories(${army_SOURCE_DIR})

add_library(insns SHARED
    Instruction.cpp RegisterArgument.cpp ImmediateArgument.cpp util.cpp
)
