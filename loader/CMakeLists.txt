include_directories(${army_SOURCE_DIR})
link_directories(${MEMORY_SRC_DIR}/memory)

add_library(loader ELFLoader.cpp)
