#include "core/CPU.h"

#include "memory/Memory.h"
#include "memory/PagedMemory.h"
#include "memory/EndianMemory.h"
#include "core/DecodingInstructionDecoder.h"

#include "instructions/Instruction.h"

CPU::CPU(Memory *external_mem, Endianness mem_endianness, bool log)
: log_(log)
{
    this->attached_mem_ = external_mem;
    this->translated_mem_ = new PagedMemory(attached_mem_, 12);
    this->value_mem_ = new EndianMemory(translated_mem_, mem_endianness);
    this->insn_decoder_ = new DecodingInstructionDecoder();
}

void CPU::set_log(bool doLog)
{
    log_ = doLog;
}

void CPU::step()
{
    addr_t insn_addr = regs_.get_reg(CPURegisters::PC);
    const Instruction *instruction = insn_decoder_->fetch_and_decode(insn_addr, *value_mem_);
    bool executable = instruction->executable(regs_);
    if (log_) {
        std::cerr << std::hex << insn_addr << std::dec << ": ";
        if (executable)
            std::cerr << "   ";
        else
            std::cerr << "XXX";
        std::cerr << *instruction << std::endl;
    }

    if (executable)
        instruction->execute(regs_, *value_mem_);

    delete instruction;
    //FIXME if instruction writes to PC, special things must happen
    //maybe RegisterArgument on write to PC will write the address just
    //so that we don't have to modify the below code
    insn_addr += 4;
    regs_.set_reg(CPURegisters::PC, insn_addr);
}

void CPU::run(addr_t start_addr)
{
    regs_.set_reg(CPURegisters::PC, start_addr);
    try {
        for(;;)
            step();
    } catch (RuntimeException &) {
        //FIXME this is just stub, should differentiate program termination (on memory read error)
        //or something really bad (like undefined instruction etc.)
    }
}