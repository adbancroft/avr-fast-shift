[![Build](https://github.com/adbancroft/avr-fast-shift/actions/workflows/build.yml/badge.svg)](https://github.com/adbancroft/avr-fast-shift/actions/workflows/build.yml)
[![Unit Tests](https://github.com/adbancroft/avr-fast-shift/actions/workflows/unit-tests.yml/badge.svg)](https://github.com/adbancroft/avr-fast-shift/actions/workflows/unit-tests.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=adbancroft_avr-fast-shift&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=adbancroft_avr-fast-shift)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=adbancroft_avr-fast-shift&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=adbancroft_avr-fast-shift)

# avr-fast-shift: optimized 32-bit shifting for avr-gcc

As of AVR-GCC 14.2.0, the code produced for unsigned 32-bit shifts with a *compile time shift distance* is very poor when using -O2 or -O3. E.g.

    rpmDelta = (toothDeltaV << 10) / (6 * toothDeltaT);

The functions provided here implement optimised left and right shifting of `uint32_t` up to 31 places.

On a physical AtMega2560, up to: 
* 35% increase in right shift performance
* 22% increase in left shift performance

See timing unit tests.

## Using the library

### Installation
The library is available in both the [Arduino Library](https://www.arduino.cc/reference/en/libraries/avr-fast-shift/) and [PlatformIO Library](https://registry.platformio.org/libraries/adbancroft/avr-fast-shift) registries. 

The library can also be cloned & included locally or included directly from GitHub (if your tooling supports it). 

### Code
1. `#include <avr-fast-shift.h>`
2. Replace all 32-bit shift operations that use a compile time constant shift distance with the equivalent shift function. I.e.
    * `a >> b` -> `rshift<b>(a)`
    * `a << b` -> `lshift<b>(a)`
    * E.g.
    * `rpmDelta = (toothDeltaV << 10) / (6 * toothDeltaT);` -> `rpmDelta = lshift<10U>(toothDeltaV) / (6 * toothDeltaT);`
