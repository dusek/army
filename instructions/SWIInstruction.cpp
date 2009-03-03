#include <cstdio>
#include <vector>

#include "instructions/SWIInstruction.h"

const static CPURegisters::Register swi_code_reg = CPURegisters::R7;
enum SYSCALL {
    SYSCALL_open = 0,
    SYSCALL_close = 1,
    SYSCALL_read = 2,
    SYSCALL_write = 3,
    SYSCALL_exit = 4
};

namespace arm {
SWIInstruction::SWIInstruction(ARM_Word swi_code, Cond cond)
:
CondInstruction(cond),
swi_code_(swi_code)
{
}

void SWIInstruction::do_execute(CPURegisters& regs, EndianMemory& mem) const
{
    ARM_Word swi_code = regs.get_reg(swi_code_reg);
    switch(swi_code) {
        case SYSCALL_exit:
            regs.end() = true;
            break;

        case SYSCALL_read:
        case SYSCALL_write:
            ARM_Word fd = regs.get_reg(CPURegisters::R0);
            if (fd != 0 && fd != 1 && fd != 2)
                // invalid fd
                regs.set_reg(CPURegisters::R0, 0xffffffff);
            else if (swi_code == SYSCALL_read && (fd == 1 || fd == 2))
                // read from stdout, stderr
                regs.set_reg(CPURegisters::R0, 0xffffffff);
            else if (swi_code == SYSCALL_write && fd == 0)
                // write to stdin
                regs.set_reg(CPURegisters::R0, 0xffffffff);
            else {
                addr_t buf = regs.get_reg(CPURegisters::R1);
                ARM_Word size = regs.get_reg(CPURegisters::R2);
                if (swi_code == SYSCALL_read) {
                    std::vector<char> bufch(size);
                    regs.set_reg(CPURegisters::R0, ::fread(&bufch[0], 1, size, stdin));
                    mem.write(buf, std::string(bufch.begin(), bufch.end()));

                } else {
                    FILE *out = (fd == 1) ? stdout : stderr;
                    regs.set_reg(CPURegisters::R0, ::fwrite(mem.read(buf, size).c_str(), 1, size, out));
                }
            }

            break;
    }
}

void SWIInstruction::fmt(std::ostream& o) const
{
    o << "SWI";
    //if (swi_code_ != 0)
        o << " #" << std::dec << swi_code_;
}

namespace is {

Instruction *SWI(ARM_Word swi_code, Cond cond)
{
    return new SWIInstruction(swi_code, cond);
}

}

}
