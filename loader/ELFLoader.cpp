#include "ELFLoader.h"

#include <cassert>

#define EI_NIDENT 16

typedef unsigned int   Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int   Elf32_Off ;
typedef unsigned int   Elf32_Word;

enum ElfHeaderIdentification {
    EI_MAG0 = 0,
    EI_MAG1 = 1,
    EI_MAG2 = 2,
    EI_MAG3 = 3,
    EI_CLASS = 4,
    EI_DATA = 5,
    EI_VERSION = 6,
    EI_PAD = 7,
};

enum {
    ELFDATANONE,
    ELFDATA2LSB,
    ELFDATA2MSB
};

enum {
    ET_NONE,
    ET_REL,
    ET_EXEC,
    ET_DYN,
    ET_CORE
};

enum {
    EM_ARM = 0x28
};


typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off  e_phoff;
    Elf32_Off  e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    Elf32_Word p_type;
    Elf32_Off  p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
} Elf32_Phdr;

enum {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR
};

enum {
    PF_X = 0x1,
    PF_W = 0x2,
    PF_R = 0x4
};

static int get_prot_for_ELF_pflags(int pflags)
{
    int ret = Memory::None;

    if (pflags & PF_X)
        ret |= Memory::Execute;
    if (pflags & PF_W)
        ret |= Memory::Write;
    if (pflags & PF_R)
        ret |= Memory::Read;

    return ret;
}

bool ELFLoader::load(std::istream &executable, Memory *memory, addr_t& load_addr)
{
    Elf32_Ehdr elf_header;
    executable.seekg(0);
    // this is awful and incorrect, but works on the compilers+platforms I tested
    // this will not work, I suppose, e.g. on some 64-bit platforms
    executable.read((char *)&elf_header, sizeof(elf_header));

    // Check ELF magic
    if ((elf_header.e_ident[0] != 0x7f) ||
        (elf_header.e_ident[1] != 'E')  ||
        (elf_header.e_ident[2] != 'L')  ||
        (elf_header.e_ident[3] != 'F')
        )
        return false;
    
    // Check endianness
    assert(elf_header.e_ident[EI_DATA] == ELFDATA2LSB);
    
    // Check this is an executable file
    assert(elf_header.e_type == ET_EXEC);

    // Check this is for ARM :-)
    assert(elf_header.e_machine == EM_ARM);

    // Check there is a page header table
    assert(elf_header.e_phoff != 0);

    int ph_entries;
    Elf32_Phdr ph_entry;
    int pos;
    std::string data;
    char *buf;
    
    executable.seekg(elf_header.e_phoff);
    
    // Iterate over program header entries, load loadable segments
    for (ph_entries = 0; ph_entries < elf_header.e_phnum; ph_entries++) {
        executable.read((char *)&ph_entry, sizeof(ph_entry));
        if ((ph_entry.p_type == PT_LOAD) && (ph_entry.p_memsz > 0)) {
            assert(ph_entry.p_filesz > 0);
            // We must load something to memory
            pos = executable.tellg();
            executable.seekg(ph_entry.p_offset);
            buf = new char[ph_entry.p_filesz];
            executable.read(buf, ph_entry.p_filesz);
            data.assign(buf, ph_entry.p_filesz);
            delete[] buf;
            const addr_t load_addr = ph_entry.p_vaddr;
            const std::size_t load_size = ph_entry.p_memsz;
            const int prot = get_prot_for_ELF_pflags(ph_entry.p_flags);
            const bool prot_nowrite = ((prot & Memory::Write) == 0);
            // We need write perm on the pages for the act of loading the executable
            memory->alloc_protect(load_addr, load_size, prot | Memory::Write);
            memory->write(load_addr, data);
            // If we added write perm above when pages should not have it, remove write perm now
            if (prot_nowrite)
                memory->alloc_protect(load_addr, load_size, prot);
            executable.seekg(pos);
            executable.seekg(elf_header.e_phentsize - sizeof(ph_entry), std::ios_base::cur);
        }
    }
    load_addr = elf_header.e_entry;
    return true;
}
