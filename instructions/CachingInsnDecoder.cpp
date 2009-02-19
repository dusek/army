#include <cassert>
#include <algorithm>
#include <functional>

#include "instructions/CachingInsnDecoder.h"

struct cache_flusher : public std::unary_function<arm::Instruction*, void>
{
    cache_flusher(InsnDecoder& slave): slave_(slave) {}
    void operator()(arm::Instruction *t)
    {
        slave_.dispose(t);
        t = 0;
    }

private:
    cache_flusher& operator=(const cache_flusher&) const;
    InsnDecoder& slave_;
};

CachingInsnDecoder::CachingInsnDecoder(InsnDecoder *slave, std::size_t insn_size, std::size_t page_size)
:
slave_(slave),
insn_size_(insn_size),
page_size_(page_size),
cur_page_addr_(0),
insn_cache_(page_size/insn_size, 0)
{
    assert(page_size_ > 0);
    assert(insn_size_ > 0);
}

arm::Instruction *CachingInsnDecoder::fetch_decode(addr_t addr, EndianMemory &mem)
{
    arm::Instruction *ret = 0;
    if (addr - cur_page_addr_ >= page_size_) {
        // flush cache
        std::for_each(insn_cache_.begin(), insn_cache_.end(), cache_flusher(*slave_));
        cur_page_addr_ = addr - (addr % page_size_);
    }

    const std::size_t page_offset = addr - cur_page_addr_;
    const std::size_t page_index = page_offset / insn_size_;

    ret = insn_cache_.at(page_index);

    if (ret == 0) {
        // not yet in cache, fetch from slave
        ret = slave_->fetch_decode(addr, mem);
        insn_cache_[page_index] = ret;
    }

    return ret;
}

void CachingInsnDecoder::dispose(arm::Instruction * /*insn*/)
{
    // no-op - we (specifically, insn_cache_) own the instrutions;
}

CachingInsnDecoder::~CachingInsnDecoder()
{
    std::for_each(insn_cache_.begin(), insn_cache_.end(), cache_flusher(*slave_));
    delete slave_;
}