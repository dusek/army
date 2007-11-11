include_directories(${army_SOURCE_DIR})
#link_directories(${army_BINARY_DIR})

add_executable(mem mem.cpp)
target_link_libraries(mem memory)

add_executable(insn-basic insn-basic.cpp)
target_link_libraries(insn-basic insns)

add_executable(testloader testloader.cpp)
target_link_libraries(testloader memory loader)
