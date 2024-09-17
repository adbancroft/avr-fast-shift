#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>
#include "avr-fast-shift.h"
#include "lambda_timer.hpp"
#include "unity_print_timers.hpp"

template <uint8_t shiftDistance>
static void test_lshift(void) {
    uint32_t value = 33333;
    char szMsg[16];
    sprintf(szMsg, "%" PRIu8, shiftDistance);
    TEST_ASSERT_EQUAL_MESSAGE(value << shiftDistance, lshift<shiftDistance>(value), szMsg);
}

static void test_LShift()
{
    test_lshift<1U>();
    test_lshift<2U>();
    test_lshift<3U>();
    test_lshift<4U>();
    test_lshift<5U>();
    test_lshift<6U>();
    test_lshift<7U>();
    test_lshift<8U>();
    test_lshift<9U>();
    test_lshift<10U>();
    test_lshift<11U>();
    test_lshift<12U>();
    test_lshift<13U>();
    test_lshift<14U>();
    test_lshift<15U>();
}

template <uint8_t shiftDistance>
static void test_rshift(void) {
    uint32_t value = 33333;
    char szMsg[16];
    sprintf(szMsg, "%" PRIu8, shiftDistance);
    TEST_ASSERT_EQUAL_MESSAGE(value >> shiftDistance, rshift<shiftDistance>(value), szMsg);
}

void test_RShift()
{
    test_rshift<1U>();
    test_rshift<2U>();
    test_rshift<3U>();
    test_rshift<4U>();
    test_rshift<5U>();
    test_rshift<6U>();
    test_rshift<7U>();
    test_rshift<8U>();
    test_rshift<9U>();
    test_rshift<10U>();
    test_rshift<11U>();
    test_rshift<12U>();
    test_rshift<13U>();
    test_rshift<14U>();
    test_rshift<15U>();
}

#if defined(USE_OPTIMIZED_SHIFTS)

static uint32_t seedValue;

constexpr uint16_t iters = 128;
constexpr uint8_t start_index = 1;
constexpr uint8_t end_index = 16;
constexpr uint8_t step = 1;

// The macros below are used to ensure the performance test functions
// have the same number of operations
//
// So we are comparing apples-to-apples

#define PERF_NATIVE_SHIFT(distance, op) if (index==(distance)) { checkSum += checkSum op (distance); }
#define PERF_NATIVE_RSHIFT(distance) PERF_NATIVE_SHIFT((distance), >>)
#define PERF_NATIVE_LSHIFT(distance) PERF_NATIVE_SHIFT((distance), <<)

#define PERF_OPTIMIZED_SHIFT(distance, op) if (index==(distance)) { checkSum += op<distance>(checkSum); }
#define PERF_OPTIMIZED_RSHIFT(distance) PERF_OPTIMIZED_SHIFT((distance), rshift)
#define PERF_OPTIMIZED_LSHIFT(distance) PERF_OPTIMIZED_SHIFT((distance), lshift)

#define PERF_TEST_FUN_BODY(shift_op) \
    if (index==1U && checkSum==0U) { checkSum = seedValue; } \
    shift_op(4U) \
    shift_op(5U) \
    shift_op(6U) \
    shift_op(7U) \
    shift_op(9U) \
    shift_op(10U) \
    shift_op(11U) \
    shift_op(12U) \
    shift_op(13U) \
    shift_op(14U) \
    shift_op(15U) \

// Force no inline, or compiler will optimize shifts away 
// (which it won't do in normal operaton when the left shift operand is unknown at compile time.)
static void __attribute__((noinline)) nativeTestRShift(uint8_t index, uint32_t &checkSum) { 
    PERF_TEST_FUN_BODY(PERF_NATIVE_RSHIFT)
};

static void __attribute__((noinline)) optimizedTestRShift(uint8_t index, uint32_t &checkSum) {
    PERF_TEST_FUN_BODY(PERF_OPTIMIZED_RSHIFT)
};

static void __attribute__((noinline)) nativeTestLShift(uint8_t index, uint32_t &checkSum) { 
    PERF_TEST_FUN_BODY(PERF_NATIVE_LSHIFT)
};

static void __attribute__((noinline)) optimizedTestLShift(uint8_t index, uint32_t &checkSum) {
    PERF_TEST_FUN_BODY(PERF_OPTIMIZED_LSHIFT)
};

#endif 

static void test_rshift_perf(void) {
#if defined(USE_OPTIMIZED_SHIFTS)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, nativeTestRShift, optimizedTestRShift);
    
    MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
    TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

    TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}


static void test_lshift_perf(void) {
#if defined(USE_OPTIMIZED_SHIFTS)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, nativeTestLShift, optimizedTestLShift);
    
    MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
    TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

    TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
#if !defined(SIMULATOR)
    delay(2000);
#endif

    UNITY_BEGIN(); 
    RUN_TEST(test_LShift);
    RUN_TEST(test_RShift);
    RUN_TEST(test_rshift_perf);
    RUN_TEST(test_lshift_perf);
    UNITY_END(); 

    // Tell SimAVR we are done
#if defined(SIMULATOR)    
    cli();
    sleep_enable();
    sleep_cpu();    
#endif    
}

void loop()
{
    // Blink to indicate end of test
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}