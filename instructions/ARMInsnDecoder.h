#ifndef ARMY_CORE_ARMINSNDECODER_H_
#define ARMY_CORE_ARMINSNDECODER_H_

#include "instructions/InsnDecoder.h"

class ARMInsnDecoder : public InsnDecoder
{
public:
    virtual arm::Instruction *fetch_decode(addr_t addr, EndianMemory& mem);
    virtual void dispose(arm::Instruction *);
};

#endif
