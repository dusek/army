#ifndef ARMY_CORE_INSNDECODER_H_
#define ARMY_CORE_INSNDECODER_H_

#include "memory/EndianMemory.h"
#include "instructions/Instruction.h"

class ARMYCORE_EXPORT InsnDecoder
{
public:
    virtual arm::Instruction *fetch_decode(addr_t addr, EndianMemory& mem) = 0;
    virtual void dispose(arm::Instruction *) = 0;

    virtual ~InsnDecoder();

protected:
    InsnDecoder();

private:
    InsnDecoder& operator=(const InsnDecoder&);
    InsnDecoder(const InsnDecoder&);
};

#endif
