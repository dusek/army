#ifndef ARMY_CORE_CPU_H_
#define ARMY_CORE_CPU_H_

#include "core/CPURegisters.h"

class Memory;
class EndianMemory;
class PagedMemory;
class InstructionDecoder;

/**
 * Abstraction of physical CPU.
 *
 * The concept is to reflect the actual CPU box - 
 * to own things that are CPU owned (e.g registers)
 * and have outlets (references) to external (attached) parts (e.g. memory)
 */
class CPU {
public:
    /**
     * Construct CPU with external_mem attached, specifying the
     * memory's endianness
     */
    CPU(Memory *external_mem, Endianness mem_endianness, bool log = false);
    void set_log(bool doLog);
    void step();
    void run(addr_t start_addr);

private:
    /**
     * CPU owned parts
     */
    CPURegisters regs_;
    InstructionDecoder *insn_decoder_;
    EndianMemory *value_mem_;
    PagedMemory *translated_mem_;
    bool log_;

    /**
     * Outlets (attached parts)
     * we are not responsible for owning them, so we keep a reference
     */
    Memory *attached_mem_;

    CPU(const CPU &cpu);
    CPU &operator=(const CPU&);
};

#endif
