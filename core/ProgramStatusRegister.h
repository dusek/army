#ifndef ARMY_CORE_PROGRAM_STATUS_REGISTER_H
#define ARMY_CORE_PROGRAM_STATUS_REGISTER_H

#include <ostream>

#include "armyconfig.h"

class ARMYCORE_EXPORT ProgramStatusRegister {
public:
    typedef enum {
        T = 0x01 << 5,
        F = 0x01 << 6,
        I = 0x01 << 7,
        Q = 0x01 << 27,
        Z = 0x01 << 28,
        C = 0x01 << 29,
        V = 0x01 << 30,
        N = 0x01 << 31
    } Bit;

    ProgramStatusRegister();

    void set_bit(Bit bit, bool set = true);
    bool get_bit(Bit bit) const;

    typedef enum {
        User,
        FIQ,
        IRQ,
        Supervisor,
        Abort,
        Undefined,
        System
    } Mode;

    Mode get_mode() const;
    void set_mode(Mode mode);

    bool operator==(const ProgramStatusRegister &) const;

private:
    ARM_Word value_;
    Mode cached_mode_;
};

ARMYCORE_EXPORT std::ostream &operator<<(std::ostream &o, ProgramStatusRegister::Bit bit);
ARMYCORE_EXPORT std::ostream &operator<<(std::ostream &o, ProgramStatusRegister::Mode mode);
ARMYCORE_EXPORT std::ostream &operator<<(std::ostream &o, ProgramStatusRegister psr);

#endif
