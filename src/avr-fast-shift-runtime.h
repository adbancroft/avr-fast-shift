#pragma once

/** @file
 * @brief Optimized uint32_t bit shifting *for AVR-GCC only*. See @ref group-opt-shift
 * 
 * @note DO NOT INCLUDE DIRECTLY. Use avr-fast-shift.h.
*/

#if defined(AFS_RUNTIME_API)

#include "avr-fast-shift-uint32.h"
#include "avr-fast-shift-int32.h"

#if defined(AFS_USE_OPTIMIZED_SHIFTS) 

/// @brief bitwise right shift optimised for the specified shift distance
/// @param a value to shift
/// @param b Number of bits to shift
/// @return a>>b 
template <typename T>
static inline T rshift(T a, uint8_t b)
{
    switch (b)
    {
        case 0: return a;
        case 1: return rshift<1U>(a);
        case 2: return rshift<2U>(a);
        case 3: return rshift<3U>(a);
        case 4: return rshift<4U>(a);
        case 5: return rshift<5U>(a);
        case 6: return rshift<6U>(a);
        case 7: return rshift<7U>(a);
        case 8: return rshift<8U>(a);
        case 9: return rshift<9U>(a);
        case 10: return rshift<10U>(a);
        case 11: return rshift<11U>(a);
        case 12: return rshift<12U>(a);
        case 13: return rshift<13U>(a);
        case 14: return rshift<14U>(a);
        case 15: return rshift<15U>(a);
        //  Note recursion here.
        default: return rshift(rshift<16>(a), (uint8_t)(b-UINT8_C(16)));
    }
}

/// @brief bitwise left shift optimised for the specified shift distance
/// @param a value to shift
/// @param b Number of bits to shift
/// @return a<<b
template <typename T>
static inline T lshift(T a, uint8_t b)
{
    switch (b)
    {
        case 0: return a;
        case 1: return lshift<1U>(a);
        case 2: return lshift<2U>(a);
        case 3: return lshift<3U>(a);
        case 4: return lshift<4U>(a);
        case 5: return lshift<5U>(a);
        case 6: return lshift<6U>(a);
        case 7: return lshift<7U>(a);
        case 8: return lshift<8U>(a);
        case 9: return lshift<9U>(a);
        case 10: return lshift<10U>(a);
        case 11: return lshift<11U>(a);
        case 12: return lshift<12U>(a);
        case 13: return lshift<13U>(a);
        case 14: return lshift<14U>(a);
        case 15: return lshift<15U>(a);
        //  Note recursion here.
        default: return lshift(lshift<16>(a), (uint8_t)(b-UINT8_C(16)));
    }
}

#else
static inline uint32_t rshift(uint32_t a, uint8_t b) { return a >> b; }
static inline uint32_t lshift(uint32_t a, uint8_t b) { return a << b; }
#endif

// These overloads are provided for completeness, but are not optimized.
// They are primarily to support template code that needs to apply shift
// to generic integral types
static inline uint8_t rshift(uint8_t a, uint8_t b) {
    return (uint8_t)(a>>b);
}
static inline uint16_t rshift(uint16_t a, uint8_t b) {
    return (uint16_t)(a>>b);
}
static inline uint8_t lshift(uint8_t a, uint8_t b) {
    return (uint8_t)(a<<b);
}
static inline uint16_t lshift(uint16_t a, uint8_t b) {
    return (uint16_t)(a<<b);
}

#endif