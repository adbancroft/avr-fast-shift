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
    test_lshift<16U>();
    test_lshift<17U>();
    test_lshift<18U>();
    test_lshift<19U>();
    test_lshift<20U>();
    test_lshift<21U>();
    test_lshift<22U>();
    test_lshift<23U>();
    test_lshift<24U>();
    test_lshift<25U>();
    test_lshift<26U>();
    test_lshift<27U>();
    test_lshift<28U>();
    test_lshift<29U>();
    test_lshift<30U>();
    test_lshift<31U>();
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
    test_rshift<16U>();
    test_rshift<17U>();
    test_rshift<18U>();
    test_rshift<19U>();
    test_rshift<20U>();
    test_rshift<21U>();
    test_rshift<22U>();
    test_rshift<23U>();
    test_rshift<24U>();
    test_rshift<25U>();
    test_rshift<26U>();
    test_rshift<27U>();
    test_rshift<28U>();
    test_rshift<29U>();
    test_rshift<30U>();
    test_rshift<31U>();
}

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

static uint32_t seedValue;
static uint8_t shiftDistance;

constexpr uint16_t iters = 2048;
constexpr uint8_t start_index = 0;
constexpr uint8_t end_index = 31;
constexpr uint8_t step = 1;

// The macros below are used to ensure the performance test functions
// have the same number of operations
//
// So we are comparing apples-to-apples

#define PERF_NATIVE_SHIFT(index, distance, op) if ((index)==(distance)) { checkSum += checkSum op (distance); }
#define PERF_NATIVE_RSHIFT(index, distance) PERF_NATIVE_SHIFT((index), (distance), >>)
#define PERF_NATIVE_LSHIFT(index, distance) PERF_NATIVE_SHIFT((index), (distance), <<)

#define PERF_OPTIMIZED_SHIFT(index, distance, op) if ((index)==(distance)) { checkSum += op<distance>(checkSum); }
#define PERF_OPTIMIZED_RSHIFT(index, distance) PERF_OPTIMIZED_SHIFT((index), (distance), rshift)
#define PERF_OPTIMIZED_LSHIFT(index, distance) PERF_OPTIMIZED_SHIFT((index), (distance), lshift)

// Randomness here is all about ensuring that the compiler doesn't optimize away the shifts
// (which it won't do in normal operaton when the shift value is unknown at compile time.)
#define PERF_TEST_FUN_BODY(shift_op) \
    if (index==0U) { \
        if (checkSum==0U) { checkSum = seedValue; randomSeed(seedValue); } \
        shiftDistance = (uint8_t)random(4, 32); \
    } else { \
        shift_op(shiftDistance, 4U) \
        shift_op(shiftDistance, 5U) \
        shift_op(shiftDistance, 6U) \
        shift_op(shiftDistance, 7U) \
        shift_op(shiftDistance, 9U) \
        shift_op(shiftDistance, 10U) \
        shift_op(shiftDistance, 11U) \
        shift_op(shiftDistance, 12U) \
        shift_op(shiftDistance, 13U) \
        shift_op(shiftDistance, 14U) \
        shift_op(shiftDistance, 15U) \
        shift_op(shiftDistance, 16U) \
        shift_op(shiftDistance, 17U) \
        shift_op(shiftDistance, 18U) \
        shift_op(shiftDistance, 19U) \
        shift_op(shiftDistance, 20U) \
        shift_op(shiftDistance, 21U) \
        shift_op(shiftDistance, 22U) \
        shift_op(shiftDistance, 23U) \
        shift_op(shiftDistance, 24U) \
        shift_op(shiftDistance, 25U) \
        shift_op(shiftDistance, 26U) \
        shift_op(shiftDistance, 27U) \
        shift_op(shiftDistance, 28U) \
        shift_op(shiftDistance, 29U) \
        shift_op(shiftDistance, 30U) \
        shift_op(shiftDistance, 31U) \
    }

static void nativeTestRShift(uint8_t index, uint32_t &checkSum) { 
    PERF_TEST_FUN_BODY(PERF_NATIVE_RSHIFT)
};

static void optimizedTestRShift(uint8_t index, uint32_t &checkSum) {
    PERF_TEST_FUN_BODY(PERF_OPTIMIZED_RSHIFT)
};

static void nativeTestLShift(uint8_t index, uint32_t &checkSum) { 
    PERF_TEST_FUN_BODY(PERF_NATIVE_LSHIFT)
};

static void optimizedTestLShift(uint8_t index, uint32_t &checkSum) {
    PERF_TEST_FUN_BODY(PERF_OPTIMIZED_LSHIFT)
};

#endif 

static void test_rshift_perf(void) {
#if defined(AFS_USE_OPTIMIZED_SHIFTS)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, nativeTestRShift, optimizedTestRShift);
    
    MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
    TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

    TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}


static void test_lshift_perf(void) {
#if defined(AFS_USE_OPTIMIZED_SHIFTS)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, nativeTestLShift, optimizedTestLShift);
    
    MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
    TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

    TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

// The macros below are used to ensure the performance test functions
// have the same number of operations
//
// So we are comparing apples-to-apples

#if defined(AFS_USE_OPTIMIZED_SHIFTS)

#define PERF_RT_NATIVE_RSHIFT(checkSum, shiftDistance) (checkSum) += (checkSum) >> (shiftDistance);
#define PERF_RT_NATIVE_LSHIFT(checkSum, shiftDistance) (checkSum) += (checkSum) << (shiftDistance);

#define PERF_RT_OPTIMIZED_RSHIFT(checkSum, shiftDistance) (checkSum) += rshift((checkSum), (shiftDistance));
#define PERF_RT_OPTIMIZED_LSHIFT(checkSum, shiftDistance) (checkSum) += lshift((checkSum), (shiftDistance));

// Randomness here is all about ensuring that the compiler doesn't optimize away the shifts
// (which it won't do in normal operaton when the shift operands are unknown at compile time.)
#define PERF_RT_TEST_FUN_BODY(shift_op) \
    if (index==0U) { \
        if (checkSum==0U) { checkSum = seedValue; randomSeed(seedValue); } \
        shiftDistance = (uint8_t)random(1, 32); \
    } else { \
        shift_op(checkSum, shiftDistance); \
    }

static inline void rtNativeTestRShift(uint8_t index, uint32_t &checkSum) { 
    PERF_RT_TEST_FUN_BODY(PERF_RT_NATIVE_RSHIFT)
};

static inline void rtOptimizedTestRShift(uint8_t index, uint32_t &checkSum) {
    PERF_RT_TEST_FUN_BODY(PERF_RT_OPTIMIZED_RSHIFT)
};

static inline void rtNativeTestLShift(uint8_t index, uint32_t &checkSum) { 
    PERF_RT_TEST_FUN_BODY(PERF_RT_NATIVE_LSHIFT)
};

static inline void rtOptimizedTestLShift(uint8_t index, uint32_t &checkSum) {
    PERF_RT_TEST_FUN_BODY(PERF_RT_OPTIMIZED_LSHIFT)
};

#endif

static void test_runtime_rshift_perf(void) {
#if defined(AFS_USE_OPTIMIZED_SHIFTS) && defined(AFS_RUNTIME_API)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, rtNativeTestRShift, rtOptimizedTestRShift);
    
    MESSAGE_TIMERS(comparison.timeA.timer, comparison.timeB.timer);
    TEST_ASSERT_EQUAL(comparison.timeA.result, comparison.timeB.result);

    TEST_ASSERT_LESS_THAN(comparison.timeA.timer.duration_micros(), comparison.timeB.timer.duration_micros());
#endif
}

static void test_runtime_lshift_perf(void) {
#if defined(AFS_USE_OPTIMIZED_SHIFTS) && defined(AFS_RUNTIME_API)
    seedValue = rand();

    auto comparison = compare_executiontime<uint8_t, uint32_t>(iters, start_index, end_index, step, rtNativeTestLShift, rtOptimizedTestLShift);
    
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
    RUN_TEST(test_runtime_rshift_perf);
    RUN_TEST(test_runtime_lshift_perf);
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