#ifndef ARMY_CORE_CACHINGINSNDECODER_H_
#define ARMY_CORE_CACHINGINSNDECODER_H_

#include <vector>

#include "instructions/InsnDecoder.h"

class CachingInsnDecoder : public InsnDecoder
{
public:
    CachingInsnDecoder(InsnDecoder *slave, std::size_t insn_size, std::size_t page_size);
    ~CachingInsnDecoder();

    virtual arm::Instruction *fetch_decode(addr_t addr, EndianMemory& mem);
    virtual void dispose(arm::Instruction *insn);

private:
    InsnDecoder *slave_;
    std::size_t insn_size_;
    std::size_t page_size_;
    addr_t cur_page_addr_;
    std::vector<arm::Instruction *> insn_cache_;
};

#endif
