#pragma once

#include <stdint.h>

#if defined(__AVR__)
#include <Arduino.h>
#else
#include <sys/time.h>
#endif

class simple_timer_t {
private:
#if defined(__AVR__)
    uint32_t start_time;
    uint32_t end_time;
#else
    struct timeval start_time;
    struct timeval end_time;
#endif

public:

    void start() {
#if defined(__AVR__)
        start_time = micros();
#else 
        gettimeofday(&start_time, nullptr);
#endif
    }

    void stop() {
#if defined(__AVR__)
        end_time = micros();
#else 
        gettimeofday(&end_time, nullptr);
#endif        
    }

    uint32_t duration_micros() const {
#if defined(__AVR__)
        return end_time-start_time;
#else 
        static constexpr unsigned long long MILLIS_PER_SEC = 1000ULL;
        static constexpr unsigned long long MICROS_PER_SEC = MILLIS_PER_SEC*1000U;
        return (uint32_t)(((end_time.tv_sec - start_time.tv_sec) * MICROS_PER_SEC) + (end_time.tv_usec - start_time.tv_usec));
#endif 
    }
};
