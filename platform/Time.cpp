#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "platform/Time.h"

Time::Time(uint64_t mcsec_since_epoch)
:
mcsec_since_epoch_(mcsec_since_epoch)
{
}

Time::Time(const Time& other)
:
mcsec_since_epoch_(other.mcsec_since_epoch_)
{
}

Time& Time::operator=(const Time& other)
{
    if (this != &other) {
        Time tmp(other);
        std::swap(*this, tmp);
    }

    return *this;
}

void Time::swap(Time &other)
{
    std::swap(mcsec_since_epoch_, other.mcsec_since_epoch_);
}

Time Time::now()
{
#ifdef _WIN32
    uint64_t mcsec_since_epoch = GetTickCount() * 1000;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    uint64_t mcsec_since_epoch = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
    return Time(mcsec_since_epoch);
}

uint64_t Time::time() const
{
    return mcsec_since_epoch_;
}

Time Time::operator-(const Time& other) const
{
    return Time(mcsec_since_epoch_ - other.mcsec_since_epoch_);
}

std::ostream& operator<<(std::ostream& o, const Time& time)
{
    o << ((double)time.time())/1000 << " ms";
    return o;
}