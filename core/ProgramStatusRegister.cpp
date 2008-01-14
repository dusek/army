#include <cassert>

#include "core/ProgramStatusRegister.h"

static const ARM_Word mode_to_int[] = {
    0x10,
    0x11,
    0x12,
    0x13,
    0x17,
    0x1b,
    0x1f
};

/*FIXME
 * The following declaration is awful copy-and-paste
 * (present in ProgramStatusRegisterTest and CPURegistersTest as well).
 * Full and clean solution is to use some smart enum template that would allow
 * iteration over enum values, etc.
 *
 * What the enum would do?
 *  1. iteration over enum values (in const_iterator STL style)
 *  2. operator<<(std::ostream &, enum ...)
 *  3. help description (short and long; useful for GUIs); these would be marked
 *       as translatable
 *  4. Associate a real value with each enum value; maybe even a pair of values
 *       - what about int and string?
 *
 * This probably can't be done cleanly in C++ even with preprocessor
 * (cleanly means without duplication, or, write the enum compactly).
 *
 * Also a monolithic vs. backwards-compatible?
 *  x monolithic - enum value would be class and would have methods;
 *                 there would be no standard enum declaration
 *  x backwards-cpt - declare enum as usual, but for each enum, implement
 *                    a set of functions (or a class with set of static methods)
 *                    that provide the needed additional services
 */
static const ProgramStatusRegister::Bit bits[] = {
    ProgramStatusRegister::N,
    ProgramStatusRegister::Z,
    ProgramStatusRegister::C,
    ProgramStatusRegister::V,
    ProgramStatusRegister::Q,
    ProgramStatusRegister::F,
    ProgramStatusRegister::I,
    ProgramStatusRegister::T
};

static const int bit_count = sizeof(bits)/sizeof(bits[0]);


ProgramStatusRegister::ProgramStatusRegister()
: value_(mode_to_int[User]),
  cached_mode_(User)
{
}

bool ProgramStatusRegister::get_bit(ProgramStatusRegister::Bit bit) const
{
    return (value_ & bit) != 0;
}

void ProgramStatusRegister::set_bit(ProgramStatusRegister::Bit bit, bool set)
{
    if (set) {
        value_ |= bit;
    } else {
        uint32_t mask = ~bit;
        value_ &= mask;
    }
}

ProgramStatusRegister::Mode ProgramStatusRegister::get_mode() const
{
    return cached_mode_;
}

void ProgramStatusRegister::set_mode(ProgramStatusRegister::Mode mode)
{
    ARM_Word mask = mode_to_int[mode];
    ARM_Word window = 0x1f;
    value_ |= (mask & window);
    value_ &= (mask | ~window);
    cached_mode_ = mode;
}

bool ProgramStatusRegister::operator==(const ProgramStatusRegister &other) const {
    if(value_ == other.value_) {
        assert(cached_mode_ == other.cached_mode_);
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &o, ProgramStatusRegister psr)
{
    o << psr.get_mode();

    for (int bit_ = 0; bit_ < bit_count; bit_++) {
        ProgramStatusRegister::Bit bit = bits[bit_];
        bool set = psr.get_bit(bit);
        if (set)
            o << " | " << bit;
    }

    return o;
}

std::ostream &operator<<(std::ostream &o, ProgramStatusRegister::Mode mode)
{
    switch(mode) {
        case ProgramStatusRegister::User:
            o << "User";
            break;
        case ProgramStatusRegister::System:
            o << "System";
            break;
        case ProgramStatusRegister::Abort:
            o << "Abort";
            break;
        case ProgramStatusRegister::Undefined:
            o << "Undefined";
            break;
        case ProgramStatusRegister::Supervisor:
            o << "Supervisor";
            break;
        case ProgramStatusRegister::IRQ:
            o << "IRQ";
            break;
        case ProgramStatusRegister::FIQ:
            o << "FIQ";
            break;
        default:
            o << "Unknown mode!";
            assert(!"Unknown mode");
            break;
    }

    return o;
}

std::ostream &operator<<(std::ostream &o, ProgramStatusRegister::Bit bit)
{
    switch(bit) {
        case ProgramStatusRegister::N:
            o << "N";
            break;
        case ProgramStatusRegister::Z:
            o << "Z";
            break;
        case ProgramStatusRegister::C:
            o << "C";
            break;
        case ProgramStatusRegister::V:
            o << "V";
            break;
        case ProgramStatusRegister::Q:
            o << "Q";
            break;
        case ProgramStatusRegister::F:
            o << "F";
            break;
        case ProgramStatusRegister::I:
            o << "I";
            break;
        case ProgramStatusRegister::T:
            o << "T";
            break;
        default:
            o << "Uknown mode!";
            assert(!"Unknown mode.");
            break;
    }

    return o;
}