#ifndef ARMY_CORE_CPU_H_
#define ARMY_CORE_CPU_H_

#include "core/CPURegisters.h"
#include "instructions/InsnDecoder.h"
#include "memory/EndianMemory.h"

/**
 * Abstraction of physical CPU.
 *
 * The concept is to reflect the actual CPU box - 
 * to own things that are CPU owned (e.g registers)
 * and have outlets (references) to external (attached) parts (e.g. memory)
 */
class ARMYCORE_EXPORT CPU {
public:
    /**
     * Construct CPU with external_mem attached, specifying the
     * memory's endianness
     */
    CPU(EndianMemory& external_mem, bool log, int argc, const char **argv);
    ~CPU();

    void step();
    void run(addr_t entry_point);

private:
    /**
     * CPU owned parts
     */
    CPURegisters regs_;
    //InstructionDecoder *insn_decoder_;
    bool log_;

    /**
     * Outlets (attached parts)
     * we are not responsible for owning them, so we keep a reference
     */
    EndianMemory& mem_;
    InsnDecoder *insn_decoder_;

    CPU(const CPU &cpu);
    CPU &operator=(const CPU&);
};

#endif
