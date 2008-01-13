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
    value_ |= mask;
    value_ &= mask;
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
    ProgramStatusRegister::Mode mode = psr.get_mode();
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

    static const struct {
        ProgramStatusRegister::Bit bit;
        const char *desc;
    } bits_desc[] = {
        { ProgramStatusRegister::N, "N" },
        { ProgramStatusRegister::Z, "Z" },
        { ProgramStatusRegister::C, "C" },
        { ProgramStatusRegister::V, "V" },
        { ProgramStatusRegister::Q, "Q" },
        { ProgramStatusRegister::I, "I" },
        { ProgramStatusRegister::F, "F" },
        { ProgramStatusRegister::T, "T" }
    };
    static const int bit_count = sizeof(bits_desc)/sizeof(bits_desc[0]);
    for (int bit_ = 0; bit_ < bit_count; bit_++) {
        if (psr.get_bit(bits_desc[bit_].bit))
            o << " | " << bits_desc[bit_].desc;
    }

    return o;
}