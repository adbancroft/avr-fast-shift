# avr-fast-shift: optimized 32-bit shifting for avr-gcc

As of AVR-GCC 14.2.0, the code produced for unsigned 32-bit shifts with a *compile time shift distance* is very poor when using -O2 or -O3. E.g.

    rpmDelta = (toothDeltaV << 10) / (6 * toothDeltaT);

The functions provided here implement optimised left and right shifting of `uint32_t` up to 16 places.

On a physical AtMega2560, up to: 
* 35% increase in right shift performance
* 22% increase in left shift performance

See timing unit tests.

## Using the library

1. `#include "fast_bit_shifts.h"`
2. Replace all 32-bit shift operations that use a compile time constant shift distance with the equivalent shift function. I.e.
    * `a >> b` -> `rshift<b>(a)`
    * `a << b` -> `lshift<b>(a)`
    * E.g.
    * `rpmDelta = (toothDeltaV << 10) / (6 * toothDeltaT);` -> `rpmDelta = lshift<10U>(toothDeltaV) / (6 * toothDeltaT);`