include_directories(${army_SOURCE_DIR})

add_library(memory SHARED 
    CollectionMemory.cpp
    EndianMemory.cpp
    PagedMemory.cpp
    StreamMemory.cpp
)
