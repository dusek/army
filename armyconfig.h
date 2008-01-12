#ifndef ARMY_CONFIG_H
#define ARMY_CONFIG_H

#if defined(__GNUC__) && __GNUC__ >= 4
#   define ARMYCORE_EXPORT __attribute__ ((visibility("default")))
#elif defined(_MSC_VER)
#   if defined(armycore_EXPORTS)
#       define ARMYCORE_EXPORT __declspec(dllexport)
#   else
#       define ARMYCORE_EXPORT __declspec(dllimport)
#   endif
#else
#   define ARMYCORE_EXPORT
#endif

//FIXME the <iostream> include is there just to fix compiling on Linux x86 (g++ 4.1.2)
//it is not needed on Mac OS X Tiger x86 (g++ 4.0.1) or Win XP XP2 (MSVC 9.0)
#include <iostream>
#include "platform/pstdint.h"

typedef uint32_t ARM_Word;
typedef uint32_t addr_t;

#endif
