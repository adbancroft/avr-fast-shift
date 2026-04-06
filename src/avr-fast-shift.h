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

#include "avr-fast-shift-uint32.h"
#include "avr-fast-shift-int32.h"
#include "avr-fast-shift-runtime.h"

// These overloads are provided for completeness, but are not optimized.
// They are primarily to support template code that needs to apply shift
// to generic integral types
template <uint8_t b> 
static inline uint8_t lshift(uint8_t a) {
    return (uint8_t)(a<<b);
}
template <uint8_t b> 
static inline int8_t lshift(int8_t a) {
    return (int8_t)(a<<b);
}
template <uint8_t b> 
static inline uint16_t lshift(uint16_t a) {
    return (uint16_t)(a<<b);
}
template <uint8_t b> 
static inline int16_t lshift(int16_t a) {
    return (int16_t)(a<<b);
}
template <uint8_t b> 
static inline uint8_t rshift(uint8_t a) {
    return (uint8_t)(a>>b);
}
template <uint8_t b> 
static inline int8_t rshift(int8_t a) {
    return (int8_t)(a>>b);
}
template <uint8_t b> 
static inline uint16_t rshift(uint16_t a) {
    return (uint16_t)(a>>b);
}
template <uint8_t b> 
static inline int16_t rshift(int16_t a) {
    return (int16_t)(a>>b);
}

///@}
