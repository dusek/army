#include "CPU.h"

#define MAP_REGS(a,b) {ARM_Register::##a,ARM_RealRegister##b}
#define SAME2(a) MAP_REGS(a,a)
#define BASE8\
    SAME2(R0),\
    SAME2(R1),\
    SAME2(R2),\
    SAME2(R3),\
    SAME2(R4),\
    SAME2(R5),\
    SAME2(R6),\
    SAME2(R7)

#define BASE12\
    BASE8,\
    SAME2(R8),\
    SAME2(R9),\
    SAME2(R10),\
    SAME2(R11)

#define CPSR_BLOCK_PRIVATE(str)\
    CPSR,\
    SPSR##str

#define CPSR\
    CPSR_BLOCK_PRIVATE()

namespace Real {
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
    SP,//R13
    LR,//R14
    PC,//R15
    SP_svc,
    LR_svc,
    SP_abt,
    LR_abt,
    SP_und,
    LR_und,
    SP_irq,
    LR_irq,
    R8_fiq,
    R9_fiq,
    R10_fiq,
    R11_fiq,
    R12_fiq,
    SP_fiq,
    LR_fiq,
    PC_fiq,//31
    CPSR,
    SPSR_svc,
    SPSR_abt,
    SPSR_und,
    SPSR_irq,
    SPSR_fiq,
    ARM_RealRegisters
} ARM_RealRegister;

