#include "PELoader.h"
#include <streambuf>

enum {
    PESigOffPos = 0x3c,
    Sz_PESigOff  = 4,
    Sz_PESig = 4,
    IMAGE_FILE_MACHINE_ARM_ = 0x01c0, // underscore because on _WIN32, we get the variant without underscore from windows.h
    IMAGE_FILE_MACHINE_THUMB_ = 0x01c2, // dtto
    Sz_MachineType = 2,
    Sz_NumberOfSections = 2,
    Sz_SizeOfOptionalHeader = 2,
    Sz_Characteristics = 2,
    IMAGE_FILE_EXECUTABLE_IMAGE_ = 0x0002,
    IMAGE_FILE_32BIT_MACHINE_ = 0x0100,
    // we will support only PE32, not PE32+
    Sz_OptHdr_Std = 28,
    Sz_OptHdr_Win = 68,
    Sz_PEMagic = 2,
    PE_Magic_32 = 0x10b,
    PE_Magic_32_Plus = 0x20b,
    IMAGE_SCN_MEM_EXECUTE_ = 0x20000000,
    IMAGE_SCN_MEM_READ_ = 0x40000000,
    IMAGE_SCN_MEM_WRITE_ = 0x80000000,
};

static int get_prot(uint32_t charac) {
    int prot = Memory::None;

    if (charac & IMAGE_SCN_MEM_EXECUTE_)
        prot |= Memory::Execute;
    if (charac & IMAGE_SCN_MEM_READ_)
        prot |= Memory::Read;
    if (charac & IMAGE_SCN_MEM_WRITE_)
        prot |= Memory::Write;

    return prot;
}

struct sect_entry {
    char name[8];
    uint32_t virt_sz;
    uint32_t virt_addr;
    uint32_t file_sz;
    uint32_t file_pos;
    uint32_t reloc_ptr;
    uint32_t linenum_ptr;
    uint16_t reloc_n;
    uint16_t linenum_n;
    uint32_t charac;

    sect_entry(std::streambuf *in) {
        //FIXME does C/C++ guarantee members in structures are stored contiguously
        // (without gaps), and in the same order as declared?

        // God (and David Bednarek) forgive me
        in->sgetn(reinterpret_cast<char *>(this), sizeof(sect_entry));
    }

    bool load(std::istream &in, Memory& mem, addr_t img_base) {
        if (virt_sz > 0) {
            addr_t sect_addr_va = img_base + virt_addr;
            int prot = get_prot(charac);
            mem.alloc_protect(sect_addr_va , virt_sz, prot | Memory::Write);
            //TODO check for file_sz > virt_sz (explained in spec)
            if (file_sz > 0) {
                std::istream::streampos pos_orig = in.tellg(); 
                if (pos_orig == std::istream::pos_type(std::istream::off_type(-1)))
                    return false;
                in.seekg(file_pos, std::ios_base::beg);
                // TODO is "bad()" the right thing to check?
                if (in.bad())
                    return false;
                char *sect_data = new char[file_sz];
                in.read(sect_data, file_sz);
                if (std::size_t(in.gcount()) < file_sz) {
                    delete[] sect_data;
                    return false;
                }
                // perhaps change "write" interface to accept const char * to
                // avoid this copying madness?
                mem.write(sect_addr_va, std::string(sect_data, file_sz));
                delete[] sect_data;
                if (!(prot & Memory::Write))
                    mem.alloc_protect(sect_addr_va, virt_sz, prot);
                in.seekg(pos_orig);
                if (in.bad())
                    return false;
            }
        }

        return true;
    }
};

const char *PE_Signature = "PE\0\0";

#define SAFE_READ(buf, ptr, sz) if (buf->sgetn(reinterpret_cast<char *>(ptr), sz) != sz) return false
#define SAFE_SEEK_FWD(buf, off) if (buf->pubseekoff(off, std::ios_base::cur) == fail_pos) return false

bool PELoader::load(std::istream &executable, Memory *mem, addr_t &addr)
{
    std::streambuf *in = executable.rdbuf();

    const std::istream::pos_type fail_pos(std::istream::off_type(-1));
    
    // get to the position where offset to PE signature is stored
    if (in->pubseekpos(PESigOffPos) == fail_pos) {
        // seek failed
        return false;
    }

    uint32_t pe_sig_off;
    // FIXME BE this will not work on Big Endian
    // PE specification does not specify whether data at 0x3c is in BE or LE
    // (it does not even specify how many bytes it spans)
    SAFE_READ(in, &pe_sig_off, Sz_PESigOff);

    // get to the position where PE signature itself is stored
    if (in->pubseekpos(pe_sig_off) == fail_pos) {
        // seek failed
        return false;
    }

    // read the PE signature
    char pe_sig[Sz_PESig];
    SAFE_READ(in, &pe_sig, Sz_PESig);

    // compare required PE signature with the one stored in file
    if (!std::equal(pe_sig, pe_sig + Sz_PESig, PE_Signature)) {
        // PE signature in file is incorrect
        return false;
    }

    // immediately after PE signature, there come to COFF File Header
    // we read what we need from it

    // read machine type - again PE spec does not tell whether information
    // in the file is stored in Little or Big endian (it tells only for 2 values
    // at all --> assuming Little endian unless specified otherwise)
    uint16_t machine_type;
    SAFE_READ(in, &machine_type, Sz_MachineType);

    // make sure this executable is for ARM
    //if (machine_type != IMAGE_FILE_MACHINE_ARM_) {
    //    // special case: if it is for Thumb, warn the user on stderr that they
    //    // cannot use Thumb instruction set
    //    if (machine_type == IMAGE_FILE_MACHINE_THUMB_) {
    //        std::cerr << "This implementation of ARM does not support Thumb instruction set." << std::endl;
    //        std::cerr << "Please recompile your program to use the ARM instruction set." << std::endl;
    //    }
    //    return false;
    //}

    // read number of sections
    uint16_t n_secs;
    SAFE_READ(in, &n_secs, Sz_NumberOfSections);

    SAFE_SEEK_FWD(in, 12);

    // read size of optional header (which is compulsory for images)
    uint16_t sz_hdr_opt;
    SAFE_READ(in, &sz_hdr_opt, Sz_SizeOfOptionalHeader);

    // read characteristics
    uint16_t charac;
    SAFE_READ(in, &charac, Sz_Characteristics);

    // check this is PE file is an executable image
    if (!(charac & IMAGE_FILE_EXECUTABLE_IMAGE_)) {
        return false;
    }

    // check this is for 32-bit word machine (which the ARMv5TE we implement is)
    if (!(charac & IMAGE_FILE_32BIT_MACHINE_)) {
        return false;
    }

    // check optional header is big enough to contain its magic
    if (sz_hdr_opt < Sz_PEMagic)
        return false;

    // read magic
    uint16_t pe_magic;
    SAFE_READ(in, &pe_magic, 2);
    // make sure this is PE32; otherwise (PE32+)
    if (pe_magic != PE_Magic_32)
        return false;

    // now make sure the first 2 sections (i.e. the fixed-size ones) are in
    // optional header
    if (sz_hdr_opt < Sz_OptHdr_Std + Sz_OptHdr_Win)
        return false;

    // skip major and minor linker version
    SAFE_SEEK_FWD(in, 2);

    // read size of code
    uint32_t sz_code;
    SAFE_READ(in, &sz_code, 4);

    // skip SizeOfInitializedData and SizeOfUninitializedData
    SAFE_SEEK_FWD(in, 8);

    // read entry point's relative virtual address (RVA)
    addr_t ep_rva;
    SAFE_READ(in, &ep_rva, 4);

    // read base of code RVA
    addr_t codebase_rva;
    SAFE_READ(in, &codebase_rva, 4);

    // skip BaseOfData
    SAFE_SEEK_FWD(in, 4);

    // read image base
    addr_t img_base;
    SAFE_READ(in, &img_base, 4);

    // skip section alignment (we don't need it since we will honor the image base)
    // skip until NumberOfRvaAndSizes
    SAFE_SEEK_FWD(in, 60);

    // read NumberOfRvaAndSizes
    uint32_t n_dirent;
    SAFE_READ(in, &n_dirent, 4);

    // consistency check - real size of optional header must be less or equal
    // to the declared size of optional header
    std::size_t sz_opthdr_dirents = sz_hdr_opt - Sz_OptHdr_Std - Sz_OptHdr_Win;
    if (sz_opthdr_dirents < 8 * n_dirent)
        return false;

    // skip the rest of optional header
    SAFE_SEEK_FWD(in, sz_opthdr_dirents);

    // read all section entries in section table and load them
    while (n_secs-- > 0)
        if (!sect_entry(in).load(executable, *mem, img_base))
            return false;

    addr = img_base + ep_rva;
    return true;
}