#pragma once
#include <Unity.h>
#include "timer.hpp"

void MESSAGE_TIMERS(const simple_timer_t &timerA, const simple_timer_t &timerB) {
    char buffer[128];
    sprintf(buffer, "Timing: %" PRIu32 ", %" PRIu32, timerA.duration_micros(), timerB.duration_micros());
    TEST_MESSAGE(buffer);
}
