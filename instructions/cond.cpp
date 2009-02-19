#include <cassert>

#include "instructions/cond.h"

bool is_cond(ProgramStatusRegister& psr, Cond cond)
{
    bool ret = false;

    switch(cond) {
        case EQ:
            ret = psr.get_bit(ProgramStatusRegister::Z);
            break;

        case NE:
            ret = !psr.get_bit(ProgramStatusRegister::Z);
            break;

        case CS:
            ret = psr.get_bit(ProgramStatusRegister::C);
            break;

        case CC:
            ret = !psr.get_bit(ProgramStatusRegister::C);
            break;

        case MI:
            ret = psr.get_bit(ProgramStatusRegister::N);
            break;

        case PL:
            ret = !psr.get_bit(ProgramStatusRegister::N);
            break;

        case VS:
            ret = psr.get_bit(ProgramStatusRegister::V);
            break;

        case VC:
            ret = !psr.get_bit(ProgramStatusRegister::V);
            break;

        case HI:
            ret = psr.get_bit(ProgramStatusRegister::C) && !psr.get_bit(ProgramStatusRegister::Z);
            break;

        case LS:
            ret = (!psr.get_bit(ProgramStatusRegister::C)) || psr.get_bit(ProgramStatusRegister::Z);
            break;

        case GE:
            ret = psr.get_bit(ProgramStatusRegister::N) == psr.get_bit(ProgramStatusRegister::V);
            break;

        case LT:
            ret = psr.get_bit(ProgramStatusRegister::N) != psr.get_bit(ProgramStatusRegister::V);
            break;

        case GT:
            ret = (!psr.get_bit(ProgramStatusRegister::Z)) && (psr.get_bit(ProgramStatusRegister::N) == psr.get_bit(ProgramStatusRegister::V));
            break;

        case LE:
            ret = psr.get_bit(ProgramStatusRegister::Z) || (psr.get_bit(ProgramStatusRegister::N) != psr.get_bit(ProgramStatusRegister::V));
            break;

        case AL:
            ret = true;
            break;

        case NV:
            ret = true;
            break;

        default:
            assert(!"Unhandled Cond case");
            break;
    }

    return ret;
}

std::ostream& operator<<(std::ostream& o, Cond cond)
{
#define HANDLE_COND(c) case c: o << #c; break
    switch(cond) {
        HANDLE_COND(EQ);
        HANDLE_COND(NE);
        HANDLE_COND(CS);
        HANDLE_COND(CC);
        HANDLE_COND(MI);
        HANDLE_COND(PL);
        HANDLE_COND(VS);
        HANDLE_COND(VC);
        HANDLE_COND(HI);
        HANDLE_COND(LS);
        HANDLE_COND(GE);
        HANDLE_COND(LT);
        HANDLE_COND(GT);
        HANDLE_COND(LE);
        case AL: break; // AL is always - usually ommited
        HANDLE_COND(NV);
        default:
            assert(!"std::ostream& operator<<(std::ostream&, Cond cond): unhandled cond value");
    }

    return o;
}

Cond cond(ARM_Word insn_word) {
    // get highest 4 bits
    ARM_Word cond_word = (insn_word >> 28) & 0x0000000f; // & 0x0 .. in case this is arithmetic right shift
    // now we are *sure* cond_word contains allowed Cond value
    Cond ret = static_cast<Cond>(cond_word);
    return ret;
}