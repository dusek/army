#ifndef ARMY_CORE_DPINSTRUCTION_H_
#define ARMY_CORE_DPINSTRUCTION_H_

#include "instructions/CondInstruction.h"
#include "core/CPURegisters.h"
#include "instructions/Arg.h"

namespace arm {

    class DPInstruction : public CondInstruction
    {
    public:
        void do_execute(CPURegisters& regs, EndianMemory& mem) const;
        void fmt(std::ostream&) const;
    protected:
        
#define ARMY_CORE_DECLARE_DP_CTOR(clsname) clsname(Cond cond, CPURegisters::Register dest_reg, Arg *op1, Arg *sh_arg, bool update_cpsr, bool store_result, bool sh_updates_c, bool reverse_args, bool has_carry, bool default_carry)        

        ARMY_CORE_DECLARE_DP_CTOR(DPInstruction);
        ~DPInstruction();
        virtual ARM_Word compute_result(ARM_Word op1, ARM_Word op2, bool carry, bool update_cv, ProgramStatusRegister& cpsr) const = 0;
        virtual const char *mnemonic() const = 0;

        bool store_result_;
        bool update_cpsr_;
        bool reverse_args_;
        bool sh_updates_c_;
        bool has_carry_;
        bool default_carry_;

        CPURegisters::Register dest_reg_;
        Arg *sh_arg_;
        Arg *op1_;
    };
}

#endif
