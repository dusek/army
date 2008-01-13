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
    value_ |= mode_to_int[mode];
    cached_mode_ = mode;
}

bool ProgramStatusRegister::operator==(const ProgramStatusRegister &other) const {
    if(value_ == other.value_) {
        assert(cached_mode_ == other.cached_mode_);
        return true;
    }
    return false;
}