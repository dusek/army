#include <map>
#include <cassert>
#include <algorithm>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
// needed for Mac OS X (and other BSDs probably too)
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#include "memory/VirtualMemory.h"

struct PageSizeInit {
    PageSizeInit(std::size_t &page_size){
#ifdef _WIN32
        SYSTEM_INFO sInf;
        GetSystemInfo(&sInf);
        page_size = sInf.dwPageSize;
#else
        page_size = sysconf(_SC_PAGESIZE);
#endif
    }
};
static std::size_t s_page_size;
static PageSizeInit pageSizeInit(s_page_size);

#ifdef _WIN32
static DWORD protect_flags(int protect) {
    DWORD dProtect = PAGE_NOACCESS;

    if (Memory::Read & protect) {
        dProtect = PAGE_READONLY;
        if (Memory::Write & protect) {
            dProtect = PAGE_READWRITE;
        }
    }
    // see remarks in the #else branch of this #ifdef about EXECUTE bit
    //if (Memory::Execute & protect)
    //    dProtect <<= 4;

    return dProtect;
}
#else
static int protect_flags(int protect) {
        int prot = PROT_NONE;
        if (Memory::Read & protect)
            prot |= PROT_READ;
        if (Memory::Write & protect)
            prot |= PROT_WRITE;
        // we are writing this for ARMY - EXECUTE has no sense, since its
        // equivalent to READ to allow the interpreter to fetch instruction
        // words. EXECUTE bit (and the whole MMU) should be rather implemented
        // in the CPU itself (just like CPURegisters, there would be a class,
        // e.g. "MMU", to handle this)
        //if (Memory::Execute & protect)
        //    prot |= PROT_EXECUTE;

        return prot;
}
#endif

// Jimmy Page rulezz :-)
class /*Jimmy*/ Page {
public:
    Page(int protect = Memory::Read | Memory::Write) {
#ifdef _WIN32
        DWORD dProtect = protect_flags(protect);
        if ((addr_ = VirtualAlloc(NULL, 1, MEM_RESERVE | MEM_COMMIT, dProtect)) == NULL) {
            throw std::bad_alloc("Could not allocate memory page");
        }
#else
        int prot = protect_flags(protect);
        if ((addr_ = mmap(NULL, s_page_size, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
            throw std::bad_alloc();
#endif
    }

    Page(const Page& other) {
        // std::auto_ptr-like copy
        addr_ = other.addr_;
        other.addr_ = 0;
    }

    void *addr() const { return addr_; }
    void protect(int protect) {
#ifdef _WIN32
        DWORD dProtect = protect_flags(protect);
        DWORD oldProtect;
        if (!VirtualProtect(addr_, 1, dProtect, &oldProtect)) {
            std::cerr << "Could not change page protection (" << std::hex << oldProtect << "->" << std::hex << dProtect << std::endl;
        }
#else
        int prot = protect_flags(protect);
        if (mprotect(addr_, s_page_size, prot) != 0) {
            std::cerr << "Could not change page protection to " << std::hex << prot << std::endl;
        }
#endif
    }

    ~Page() {
        if (addr_) {
#ifdef _WIN32

            if (!VirtualFree(addr_, 0, MEM_RELEASE)) {
                DWORD err = GetLastError();
                std::cerr << "Error releasing page " << std::hex << addr_ << ": GetLastError returned " << err << std::endl;
            }
#else
        if (munmap(addr_, s_page_size) != 0)
            std::cerr << "Error unmapping page " << std::hex << addr_ << std::endl;
#endif
        }
    }

private:
    mutable void *addr_;
};

static addr_t get_page_addr(addr_t addr)
{
    return addr - (addr % s_page_size);
}

struct VirtualMemory::VMPrivate
{
    struct vmmap_entry {
        vmmap_entry(const Page& page_, int protect_): page(page_), protect(protect_) {}
        Page page;
        int protect; // cache protect bits so we don't have to query OS for them all the time
    };
    /*
    ** Contains mapping of Virtual(ARM) addresses to RIAA Page objects with protect bits as well
    */
    typedef std::map<addr_t, vmmap_entry> vmmap_type;
    vmmap_type vmmap; // this is "tlb"

    /*
    ** Creates a Virtual(ARM) -> Physical(host) memory address mapping.
    ** Creates mapping for virtual address "addr". "addr" must be aligned
    ** on boundary of size s_page_size.
    ** If page exists, it is re-protected with "protect" unless "protect" is -1
    ** or "protect" is equal to current page protection.
    ** If page does not exist, it is created with protection "protect",
    ** unless "protect" is -1, in which case it creates a page with default
    ** protection Read | Write.
    */
    void *alloc_protect(addr_t addr, int protect) {
        assert((addr % s_page_size) == 0);

        vmmap_type::iterator it;

        if ((it = vmmap.find(addr)) != vmmap.end()) {
            // page already exists
            if ((protect != -1) && (protect != it->second.protect)) {
                // requested protection differs from current one
                it->second.protect = protect;
                it->second.page.protect(protect);
            }
        } else {
            // page does not exist
            int prot = protect;
            if (protect == -1)
                prot = Memory::Read | Memory::Write;
            Page page(prot);
            vmmap_entry page_entry(page, prot);
            vmmap_type::value_type value(addr, page_entry);
            it = vmmap.insert(it, value);
        }

        return it->second.page.addr();
    }

    template<typename T>
    void gather_scatter(addr_t addr, std::size_t size, T scatter_action)
    {
        std::size_t left = size;

        const addr_t first_page_addr = get_page_addr(addr);
        const std::size_t first_page_off = addr - first_page_addr;

        addr_t page_addr;
        for (addr_t next_page_addr = first_page_addr; left != 0;) {
            page_addr = next_page_addr;
            next_page_addr += s_page_size;
            char * host_page_addr = (char *)alloc_protect(page_addr, -1);
            char *host_addr = host_page_addr;
            std::size_t req_sz = s_page_size;
            if (page_addr == first_page_addr) {
                req_sz -= first_page_off;
                host_addr += first_page_off;
            }
            req_sz = std::min(left, req_sz);

            scatter_action(host_addr, req_sz);
            left -= req_sz;
        }
    }
};

VirtualMemory::VirtualMemory()
: pimpl_(new VMPrivate)
{}

void VirtualMemory::alloc_protect(addr_t addr, std::size_t size, int protect)
{
    addr_t first_page_addr = get_page_addr(addr);
    addr_t last_addr = addr + size - 1;
    addr_t last_page_addr = get_page_addr(last_addr);
    for (addr_t page_addr = first_page_addr; page_addr <= last_page_addr; page_addr += s_page_size)
        pimpl_->alloc_protect(addr, protect);
}

int VirtualMemory::get_protect(addr_t addr) const
{
    int prot = -1;

    VMPrivate::vmmap_type& vmmap = pimpl_->vmmap;

    addr_t page_addr = get_page_addr(addr);
    VMPrivate::vmmap_type::const_iterator it;

    if ((it = vmmap.find(page_addr)) != vmmap.end())
        prot = it->second.protect;

    return prot;
}

/*
** "Scatters" single request on a contiguous area of virtual memory [addr, addr+size)
** into possibly multiple requests on different pages of host memory,
** _in the order_ original virtual memory area maps to these host pages.
** Calls "ftor" for each request on host memory.
** Requirements on T:
**  * T::operator()(const char *host_addr, std::size_t req_sz)
**     - [host_addr, host_addr+req_sz) is the region in host memory
**       that recieves
*/

template<typename RandomIt>
struct write_scatter_action : public std::binary_function<addr_t, std::size_t, void>
{
    write_scatter_action(RandomIt begin): begin_(begin){}
    void operator()(char *host_addr, std::size_t req_sz) {
        const RandomIt end = begin_ + req_sz;
        std::copy(begin_, end, host_addr);
        begin_ = end;
    }
private:
    RandomIt begin_;
};

template<typename RandomIt>
static write_scatter_action<RandomIt> make_write_scatter_action(RandomIt it)
{
    return write_scatter_action<RandomIt>(it);
}

void VirtualMemory::write(addr_t addr, const std::string &data)
{
    pimpl_->gather_scatter(addr, data.size(), make_write_scatter_action(data.begin()));
}

template<typename OutputIt>
struct read_scatter_action : public std::binary_function<addr_t, std::size_t, void>
{
    read_scatter_action(OutputIt begin): begin_(begin){}
    void operator()(const char *host_addr, std::size_t req_sz) {
        begin_ = std::copy(host_addr, host_addr + req_sz, begin_);
    }
private:
    OutputIt begin_;
};

template<typename OutputIt>
static read_scatter_action<OutputIt> make_read_scatter_action(OutputIt it)
{
    return read_scatter_action<OutputIt>(it);
}

std::string VirtualMemory::read(addr_t addr, std::size_t bytes)
{
    std::string ret(bytes, 0);
    pimpl_->gather_scatter(addr, bytes, make_read_scatter_action(ret.begin()));
    return ret;
}

VirtualMemory::~VirtualMemory()
{
    delete pimpl_;
}

std::size_t VirtualMemory::page_size()
{
    return s_page_size;
}
