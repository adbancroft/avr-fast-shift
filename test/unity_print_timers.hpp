#pragma once
#include <unity.h>
#include "timer.hpp"

static inline void MESSAGE_TIMERS(const simple_timer_t &timerA, const simple_timer_t &timerB) {
    auto aTime = timerA.duration_micros();
    auto bTime = timerB.duration_micros();
    auto percent = bTime * 100U / aTime;

    TEST_PRINTF("Timing: %lu, %lu, %lu%%", aTime, bTime, percent);
}
