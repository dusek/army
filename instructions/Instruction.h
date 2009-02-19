#ifndef ARMY_CORE_INSTRUCTION_H_
#define ARMY_CORE_INSTRUCTION_H_

#include <ostream>

#include "armyconfig.h"
#include "core/CPURegisters.h"
#include "memory/EndianMemory.h"

namespace arm {

    class Instruction
    {
    public:
        virtual ~Instruction();
        // instruction instance = "executable description of change of the state of the CPU"

        // executable
        virtual void execute(CPURegisters&, EndianMemory&) const = 0;

        // description
        virtual void fmt(std::ostream&) const = 0;

        // and nothing more!!!

    protected:
        Instruction();
    };

    std::ostream& operator<<(std::ostream&, const Instruction&);
}

#endif
