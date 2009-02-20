#ifndef ARMY_CORE_TIME_H_
#define ARMY_CORE_TIME_H_

#include <utility>

#include "armyconfig.h"

class ARMYCORE_EXPORT Time
{
public:
    Time(const Time& other);
    Time& operator=(const Time& other);
    static Time now();
    void swap(Time& other);
    uint64_t time() const;
    Time operator-(const Time& other) const;

private:
    Time(uint64_t sec_since_epoch);
    uint64_t mcsec_since_epoch_;
};

namespace std {
    template<>
    void swap(Time& t1, Time& t2)
    {
        t1.swap(t2);
    }
}

ARMYCORE_EXPORT std::ostream& operator<<(std::ostream& o, const Time& time);

#endif
