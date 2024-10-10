#pragma once

/** @file
 * @brief Optimized uint32_t bit shifting *for AVR-GCC only*. See @ref group-opt-shift
*/

#include <stdint.h>

/// @defgroup group-opt-shift Optimised bitwise shifts
///
/// @brief As of AVR-GCC 14.2.0, the code produced for 32-bit shifts with a *compile time shift distance* is very poor when using -O2 or -O3.
/// The templates here implement optimised shifting. Average 35% increase in shift performance on AtMega2560: see timing unit test.
///
/// Inspired by https://aykevl.nl/2021/02/avr-bitshift
///
/// @note The assembler was generated using clang 17.0.1 cross compiling: -O3 --target=avr -mmcu=atmega2560. See https://godbolt.org/z/71cPnMYqs
///
/// Usage:
/// @code
///      rpmDelta = lshift<10>(toothDeltaV) / (6 * toothDeltaT);
/// @endcode
///
/// If there is no overload for a certain shift, that's because GCC produced decent ASM
/// in that case.
/// 
/// @note Code is usable on all architectures, but the optimization only applies to AVR-GCC.
/// Other compilers will see a standard bitwise shift.
/// @{
    
/// @brief Preprocessor flag to turn on optimized shifts.
/// If not set eternally, will be automatically set for GCC on AVR.
#if !defined(AFS_USE_OPTIMIZED_SHIFTS)
#if (defined(__GNUC__) && defined(__AVR__)) || defined(DOXYGEN_DOCUMENTATION_BUILD)
#define AFS_USE_OPTIMIZED_SHIFTS
#endif
#endif

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/// @{
/// @brief uint32_t bitwise left shift optimised for the specified shift distance  
/// 
/// @param a value to shift
/// @return uint32_t a<<b
template <uint8_t b> 
static inline uint32_t lshift(uint32_t a) {
    // Template is specialized for shifts of 1-16 bits below.
    static_assert(b>16, "Missing template specialization");
    // This is the default, for shifts of 17 or more bits.
    // Shift by 16, then by the remaining amount.
    return lshift<b-16U>(lshift<16>(a));
}

template <> inline uint32_t lshift<1U>(uint32_t a) {
    return a << 1U;
}
template <> inline uint32_t lshift<2U>(uint32_t a) {
    return a << 2U;
}
template <> inline uint32_t lshift<3U>(uint32_t a) {
    return a << 3U;
}
template <> inline uint32_t lshift<8U>(uint32_t a) {
    return a << 8U;
}
template <> inline uint32_t lshift<16U>(uint32_t a) {
    return a << 16U;
}
template <> inline uint32_t lshift<24U>(uint32_t a) {
    return a << 24U;
}

template <> inline uint32_t lshift<4U>(uint32_t a) {
    asm(
        "swap    %D0\n"
        "andi    %D0, 240\n"
        "swap    %C0\n"
        "eor     %D0, %C0\n"
        "andi    %C0, 240\n"
        "eor     %D0, %C0\n"
        "swap    %B0\n"
        "eor     %C0, %B0\n"
        "andi    %B0, 240\n"
        "eor     %C0, %B0\n"
        "swap    %A0\n"
        "eor     %B0, %A0\n"
        "andi    %A0, 240\n"
        "eor     %B0, %A0\n"
        : "=d" (a) 
        : "0" (a) 
        :
    );

    return a;
}

template <> inline uint32_t lshift<5U>(uint32_t a) {
    asm(
        "swap    %D0\n"
        "andi    %D0, 240\n"
        "swap    %C0\n"
        "eor     %D0, %C0\n"
        "andi    %C0, 240\n"
        "eor     %D0, %C0\n"
        "swap    %B0\n"
        "eor     %C0, %B0\n"
        "andi    %B0, 240\n"
        "eor     %C0, %B0\n"
        "swap    %A0\n"
        "eor     %B0, %A0\n"
        "andi    %A0, 240\n"
        "eor     %B0, %A0\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<6U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "mov     r18, __zero_reg__\n"
        "ror     r18\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "ror     r18\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     r19, %A0\n"
        "movw    %A0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline uint32_t lshift<7U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "mov     r18, __zero_reg__\n"
        "ror     r18\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     r19, %A0\n"
        "movw    %A0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline uint32_t lshift<9U>(uint32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     %B0, %A0\n"
        "mov     %A0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<10U>(uint32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     %B0, %A0\n"
        "mov     %A0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<11U>(uint32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     %B0, %A0\n"
        "mov     %A0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<12U>(uint32_t a) {
    asm(
        "swap    %C0\n"
        "andi    %C0, 240\n"
        "swap    %B0\n"
        "eor     %C0, %B0\n"
        "andi    %B0, 240\n"
        "eor     %C0, %B0\n"
        "swap    %A0\n"
        "eor     %B0, %A0\n"
        "andi    %A0, 240\n"
        "eor     %B0, %A0\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     %B0, %A0\n"
        "mov     %A0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<13U>(uint32_t a) {
    asm(
        "swap    %C0\n"
        "andi    %C0, 240\n"
        "swap    %B0\n"
        "eor     %C0, %B0\n"
        "andi    %B0, 240\n"
        "eor     %C0, %B0\n"
        "swap    %A0\n"
        "eor     %B0, %A0\n"
        "andi    %A0, 240\n"
        "eor     %B0, %A0\n"
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "mov     %D0, %C0\n"
        "mov     %C0, %B0\n"
        "mov     %B0, %A0\n"
        "mov     %A0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t lshift<14U>(uint32_t a) {
    asm(
        "movw    r18, %A0\n"
        "lsr     %C0\n"
        "ror     r19\n"
        "ror     r18\n"
        "mov     %B0, __zero_reg__\n"
        "ror     %B0\n"
        "lsr     %C0\n"
        "ror     r19\n"
        "ror     r18\n"
        "ror     %B0\n"
        "mov     %A0, __zero_reg__\n"
        "movw    %C0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline uint32_t lshift<15U>(uint32_t a) {
    asm(
        "movw    r18, %A0\n"
        "lsr     %C0\n"
        "ror     r19\n"
        "ror     r18\n"
        "mov     %B0, __zero_reg__\n"
        "ror     %B0\n"
        "mov     %A0, __zero_reg__\n"
        "movw    %C0, r18\n"
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
static inline uint32_t lshift(uint32_t a) { 
   return a << b; 
}
#endif

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/// @{
/// @brief uint32_t bitwise right shift optimised for the specified shift distance  
/// 
/// @param a value to shift
/// @return uint32_t a<<b
template <uint8_t b> 
static inline uint32_t rshift(uint32_t a) {
    // Template is specialized for shifts of 1-16 bits below.
    static_assert(b>16, "Missing template specialization");
    // This is the default, for shifts of 17 or more bits.
    // Shift by 16, then by the remaining amount.
    return rshift<b-16U>(rshift<16>(a));
}

template <> inline uint32_t rshift<1U>(uint32_t a) {
    return a >> 1U;
}
template <> inline uint32_t rshift<2U>(uint32_t a) {
    return a >> 2U;
}
template <> inline uint32_t rshift<8U>(uint32_t a) {
    return a >> 8U;
}
template <> inline uint32_t rshift<16U>(uint32_t a) {
    return a >> 16U;
}
template <> inline uint32_t rshift<24U>(uint32_t a) {
    return a >> 24U;
}

template <> inline uint32_t rshift<3U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<4U>(uint32_t a) {
    asm(
        "swap    %A0\n"
        "andi    %A0, 15\n"
        "swap    %B0\n"
        "eor     %A0, %B0\n"
        "andi    %B0, 15\n"
        "eor     %A0, %B0\n"
        "swap    %C0\n"
        "eor     %B0, %C0\n"
        "andi    %C0, 15\n"
        "eor     %B0, %C0\n"
        "swap    %D0\n"
        "eor     %C0, %D0\n"
        "andi    %D0, 15\n"
        "eor     %C0, %D0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<5U>(uint32_t a) {
    asm(
        "swap    %A0\n"
        "andi    %A0, 15\n"
        "swap    %B0\n"
        "eor     %A0, %B0\n"
        "andi    %B0, 15\n"
        "eor     %A0, %B0\n"
        "swap    %C0\n"
        "eor     %B0, %C0\n"
        "andi    %C0, 15\n"
        "eor     %B0, %C0\n"
        "swap    %D0\n"
        "eor     %C0, %D0\n"
        "andi    %D0, 15\n"
        "eor     %C0, %D0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "ror     %A0\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<6U>(uint32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        "mov     r19, __zero_reg__\n"
        "rol     r19\n"
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

template <> inline uint32_t rshift<7U>(uint32_t a) {
    asm(
        "lsl     %A0\n"
        "rol     %B0\n"
        "rol     %C0\n"
        "rol     %D0\n"
        "mov     r19, __zero_reg__\n"
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

template <> inline uint32_t rshift<9U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     %C0, %D0\n"
        "mov     %D0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<10U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n" 
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     %C0, %D0\n"
        "mov     %D0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<11U>(uint32_t a) {
    asm(
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     %C0, %D0\n"
        "mov     %D0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<12U>(uint32_t a) {
    asm(
        "swap    %B0\n"
        "andi    %B0, 15\n"
        "swap    %C0\n"
        "eor     %B0, %C0\n"
        "andi    %C0, 15\n"
        "eor     %B0, %C0\n"
        "swap    %D0\n"
        "eor     %C0, %D0\n"
        "andi    %D0, 15\n"
        "eor     %C0, %D0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     %C0, %D0\n"
        "mov     %D0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<13U>(uint32_t a) {
    asm(
        "swap    %B0\n"
        "andi    %B0, 15\n"
        "swap    %C0\n"
        "eor     %B0, %C0\n"
        "andi    %C0, 15\n"
        "eor     %B0, %C0\n"
        "swap    %D0\n"
        "eor     %C0, %D0\n"
        "andi    %D0, 15\n"
        "eor     %C0, %D0\n"
        "lsr     %D0\n"
        "ror     %C0\n"
        "ror     %B0\n"
        "mov     %A0, %B0\n"
        "mov     %B0, %C0\n"
        "mov     %C0, %D0\n"
        "mov     %D0, __zero_reg__\n"
        : "=d" (a) 
        : "0" (a) 
        : 
    );

    return a;
}

template <> inline uint32_t rshift<14U>(uint32_t a) {
    asm(
        "movw    r18, %C0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "mov     %C0, __zero_reg__\n"
        "rol     %C0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "rol     %C0\n"
        "mov     %D0, __zero_reg__\n"
        "movw    %A0, r18\n"
        : "=d" (a) 
        : "0" (a) 
        : "r18", "r19"
    );

    return a;
}

template <> inline uint32_t rshift<15U>(uint32_t a) {
    asm(
        "movw    r18, %C0\n"
        "lsl     %B0\n"
        "rol     r18\n"
        "rol     r19\n"
        "mov     %C0, __zero_reg__\n"
        "rol     %C0\n"
        "mov     %D0, __zero_reg__\n"
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
static inline uint32_t rshift(uint32_t a) { 
    return a >> b; 
}
#endif

#if defined(AFS_USE_OPTIMIZED_SHIFTS) && defined(AFS_RUNTIME_API)

static inline uint32_t rshift(uint32_t a, uint8_t b)
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


static inline uint32_t lshift(uint32_t a, uint8_t b)
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

///@}
