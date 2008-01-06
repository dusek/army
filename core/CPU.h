#ifndef __CPU_H__
#define __CPU_H__

class Instruction;
class Memory;

#include <bitset>

#include "platform/pstdint.h"

#define ENUM_LAST(e,value) enum {\
   e##s = value + 1\
} 

typedef enum {
    User,
    System,
    Supervisor,
    Abort,
    Undefined,
    IRQ,
    FIQ
} ARM_Mode;

ENUM_LAST(ARM_Mode,FIQ);

#define PROPERTY_ACCESS(name,type)\
    type get_##name(void);\
    void set_##name(type value)

typedef std::bitset<32> CPSR_Reg;

typedef enum {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    SP=R13,
    LR=R14,
    PC=R15
} ARM_Register;

typedef enum {
    T=5,
    F,
    I,
    Q=27,
    V,
    C,
    Z,
    N
} CPSR_Bit;

ENUM_LAST(ARM_Register,PC);

class CPU {
public:
    CPU(Memory *mem);
    void step();
    void execute(Instruction *insn);
    void run(uint32_t breakpoint);
    
    void     set_reg(ARM_Register, uint32_t){/*FIXME stub*/}
    uint32_t get_reg(ARM_Register){/*FIXME stub*/ return 0;}
    
    PROPERTY_ACCESS(mode,ARM_Mode);

    CPSR_Reg &cpsr();
    CPSR_Reg &spsr();

private:

    struct CPUPrivate;
    CPUPrivate const *d;
};

#endif
