#pragma once

#pragma once

/** @file
 * @brief Optimized int32_t bit shifting *for AVR-GCC only*. See @ref group-opt-shift
 * 
 * @note DO NOT INCLUDE DIRECTLY. Use avr-fast-shift.h.
*/

#include <stdint.h>
#include "avr-fast-shift-uint32.h"

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/// @{
/// @brief bitwise left shift optimised for the specified shift distance
/// @tparam b Number of bits to shift
/// @param a value to shift
/// @return a<<b
template <uint8_t b> 
static inline int32_t lshift(int32_t a) {
    // Signed left shift is the same as unsigned
    return (int32_t)lshift<b>((uint32_t)a);
}
///@}

#pragma GCC diagnostic pop

#else
template <uint8_t b> 
static inline int32_t lshift(int32_t a) { 
   return a << b; 
}
#endif

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/// @{
/// @brief bitwise right shift optimised for the specified shift distance
/// @tparam b Number of bits to shift
/// @param a value to shift
/// @return a<<b
template <uint8_t b> 
static inline int32_t rshift(int32_t a) {
    // Template is specialized for shifts of 1-16 bits below.
    static_assert(b>16, "Missing template specialization");
    // This is the default, for shifts of 17 or more bits.
    // Shift by 16, then by the remaining amount.
    return rshift<b-16U>(rshift<16>(a));
}

template <> inline int32_t rshift<1U>(int32_t a) {
    return a >> 1U;
}
template <> inline int32_t rshift<2U>(int32_t a) {
    return a >> 2U;
}
template <> inline int32_t rshift<8U>(int32_t a) {
    return a >> 8U;
}
template <> inline int32_t rshift<16U>(int32_t a) {
    return a >> 16U;
}
template <> inline int32_t rshift<24U>(int32_t a) {
    return a >> 24U;
}

template <> inline int32_t rshift<3U>(int32_t a) {
    asm(
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline int32_t rshift<4U>(int32_t a) {
    asm(
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline int32_t rshift<5U>(int32_t a) {
    asm(
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline int32_t rshift<6U>(int32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        "sbc     r19, r19\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        "rol     r19\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<7U>(int32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        "sbc     r19, r19\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<9U>(int32_t a) {
    asm(
        "mov     r19, %D0\n"
        "lsl     r19\n"
        "sbc     r19, r19\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<10U>(int32_t a) {
    asm(
        "mov     r19, %D0\n"
        "lsl     r19\n"
        "sbc     r19, r19\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<11U>(int32_t a) {
    asm(
        "mov     r19, %D0\n"
        "lsl     r19\n"
        "sbc     r19, r19\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<12U>(int32_t a) {
    asm(
        "mov     r19, %D0\n"
        "lsl     r19\n"
        "sbc     r19, r19\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<13U>(int32_t a) {
    asm(
        "mov     r19, %D0\n"
        "lsl     r19\n"
        "sbc     r19, r19\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "asr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     r18, %D0\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<14U>(int32_t a) {
    asm(
        "movw    r18, %C0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "sbc     %D0, %D0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "mov     %C0, %D0\n"
        "rol     %C0\n"
        "movw    %A0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline int32_t rshift<15U>(int32_t a) {
    asm(
        "movw    r18, %C0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "sbc     %C0, %C0\n"
        "mov     %D0, %C0\n"
        "movw    %A0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

///@}

#pragma GCC diagnostic pop

#else
template <uint8_t b> 
static inline int32_t rshift(int32_t a) { 
    return a >> b; 
}
#endif