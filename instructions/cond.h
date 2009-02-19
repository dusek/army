#ifndef ARMY_CORE_COND_H_
#define ARMY_CORE_COND_H_

#include <ostream>

#include "core/ProgramStatusRegister.h"

enum Cond {
    EQ,
    NE,
    CS,
    CC,
    MI,
    PL,
    VS,
    VC,
    HI,
    LS,
    GE,
    LT,
    GT,
    LE,
    AL,
    NV
};

bool is_cond(ProgramStatusRegister& psr, Cond cond);
std::ostream& operator<<(std::ostream&, Cond cond);
Cond cond(ARM_Word insn_word);

#endif
