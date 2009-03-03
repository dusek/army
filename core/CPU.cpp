#include <string>
#include <bitset>

#include "core/CPU.h"

#include "memory/Memory.h"
#include "memory/PagedMemory.h"
#include "memory/EndianMemory.h"
#include "instructions/CachingInsnDecoder.h"
#include "instructions/ARMInsnDecoder.h"

#include "instructions/Instruction.h"

CPU::CPU(EndianMemory& external_mem, bool log, bool log_dumpregs, int argc, const char **argv)
:
log_(log),
log_dump_(log_dumpregs),
mem_(external_mem),
insn_decoder_(new CachingInsnDecoder(new ARMInsnDecoder, 4, 4096)),
pc_counter_(0)
{
    regs_.set_reg(CPURegisters::SP, 0);

    // copy arguments (argc,argv) to memory
    const addr_t addr_base = 0x37240000; // "randomness"
    addr_t addr_arg = addr_base; // "randomness"

    std::size_t total_size = 0;
    int arg = 0;
    for (; arg < argc; ++arg) {
        const char *argstr = argv[arg];
        total_size += ::strlen(argstr) + 1;
    }

    mem_.alloc_protect(addr_base, total_size, Memory::Read | Memory::Write);

    for (arg = 0; arg < argc; ++arg) {
        const char *argstr = argv[arg];
        const std::size_t size = ::strlen(argstr) + 1;
        mem_.write(addr_arg, std::string(argstr, argstr + size));
        mem_.write_value(addr_base + 4*arg, addr_arg);
        addr_arg += size;
    }

    mem_.alloc_protect(addr_base, total_size, Memory::Read);
    regs_.set_reg(CPURegisters::R0, argc);
    regs_.set_reg(CPURegisters::R1, addr_base);
}

void CPU::step()
{
    addr_t insn_addr = regs_.get_reg(CPURegisters::PC);
    arm::Instruction *instruction = insn_decoder_->fetch_decode(insn_addr, mem_);

    if (instruction) {
        if (log_) {
            std::cerr << std::hex << insn_addr << ": " << *instruction << std::endl;
        }

        instruction->execute(regs_, mem_);

        if (log_dump_) {
            regs_.dump(std::cerr);
            std::cerr << std::endl;
            std::cerr << "CPSR: " << regs_.status_reg(CPURegisters::CPSR) << std::endl;
            std::cerr << std::endl;
        }

        insn_decoder_->dispose(instruction);
        if (!regs_.is_PC_dirty()) {
            insn_addr += 4;
            regs_.set_reg(CPURegisters::PC, insn_addr);
            regs_.is_PC_dirty();
        }
    } else {
        ARM_Word insn_word = mem_.read_value(insn_addr);
        std::cerr << std::hex << insn_addr << ": Invalid instruction word: " << std::hex << insn_word  << " (" << std::bitset<32>(insn_word) << ")" << std::endl;
        regs_.end() = true;
    }

    ++pc_counter_;
}

uint64_t CPU::pc_counter() const
{
    return pc_counter_;
}

void CPU::run(addr_t entry_point)
{
    regs_.set_reg(CPURegisters::PC, entry_point);
    regs_.is_PC_dirty();
    while (!regs_.end()) {
        step();
    }
}

CPU::~CPU()
{
    delete insn_decoder_;
}
