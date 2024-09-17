/*
 avr-fast-div example
*/
#include <Arduino.h>
#include <stdint.h>
#include "avr-fast-shift.h"

static uint32_t seedValue;

// Forward declarations
static void nativeTestRShift(uint8_t index, uint32_t & checkSum);
static void optimizedTestRShift(uint8_t index, uint32_t & checkSum);

void setup() {
  Serial.begin(9600);   //send and receive at 9600 baud  
  pinMode(LED_BUILTIN, OUTPUT);

  seedValue = rand();

  Serial.println("Beginning test...");

  constexpr uint16_t iters = 128;
  constexpr uint8_t start_index = 1;
  constexpr uint8_t end_index = 16;

  // Built in "/" operator
  uint32_t shiftCheckSum = 0UL;
  uint32_t shiftStartTime = micros();
  for (uint16_t i = 0; i < iters; ++i) {
    for (uint8_t j = start_index; j < end_index; ++j) {
     nativeTestRShift(j, shiftCheckSum);
    }
  }
  uint32_t shiftEndTime = micros();
  uint32_t shiftDuration = shiftEndTime-shiftStartTime;

  // fast_div()
  uint32_t fastShiftCheckSum = 0UL;
  uint32_t fastShiftStartTime = micros();
  for (uint16_t i = 0; i < iters; ++i) {
    for (uint8_t j = start_index; j < end_index; ++j) {
      optimizedTestRShift(j, fastShiftCheckSum);
    }
  }
  uint32_t fastShiftEndTime = micros();
  uint32_t fastShiftDuration = fastShiftEndTime-fastShiftStartTime;

  char msg[128];
  snprintf(msg, sizeof(msg)-1, "Shift Checksum: %" PRIu32 ", fast_shift Checksum: %" PRIu32, shiftCheckSum, fastShiftCheckSum);
  Serial.println(msg);
  snprintf(msg, sizeof(msg)-1, "Shift Duration: %" PRIu32 ", fast_shift Duration: %" PRIu32, shiftDuration, fastShiftDuration);
  Serial.println(msg);
  uint16_t percentDelta = (uint16_t)((fastShiftDuration * 100U) / shiftDuration);
  snprintf(msg, sizeof(msg)-1, "fast_shift() took %" PRIu16 "%% less time than the shift operator", 100-percentDelta);
  Serial.println(msg);
}

void loop() {

    // Blink to indicate end of test
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}

static void __attribute__((noinline)) nativeTestRShift(uint8_t index, uint32_t & checkSum) {
  if (index == 1U && checkSum == 0U) {
    checkSum = seedValue;
  }
  if (index == 4U) {
    checkSum += checkSum >> 4U;
  }
  if (index == 5U) {
    checkSum += checkSum >> 5U;
  }
  if (index == 6U) {
    checkSum += checkSum >> 6U;
  }
  if (index == 7U) {
    checkSum += checkSum >> 7U;
  }
  if (index == 9U) {
    checkSum += checkSum >> 9U;
  }
  if (index == 10U) {
    checkSum += checkSum >> 10U;
  }
  if (index == 11U) {
    checkSum += checkSum >> 11U;
  }
  if (index == 12U) {
    checkSum += checkSum >> 12U;
  }
  if (index == 13U) {
    checkSum += checkSum >> 13U;
  }
  if (index == 14U) {
    checkSum += checkSum >> 14U;
  }
  if (index == 15U) {
    checkSum += checkSum >> 15U;
  }
}

static void __attribute__((noinline)) optimizedTestRShift(uint8_t index, uint32_t & checkSum) {
  if (index == 1U && checkSum == 0U) {
    checkSum = seedValue;
  }
  if (index == 4U) {
    checkSum += rshift < 4U > (checkSum);
  }
  if (index == 5U) {
    checkSum += rshift < 5U > (checkSum);
  }
  if (index == 6U) {
    checkSum += rshift < 6U > (checkSum);
  }
  if (index == 7U) {
    checkSum += rshift < 7U > (checkSum);
  }
  if (index == 9U) {
    checkSum += rshift < 9U > (checkSum);
  }
  if (index == 10U) {
    checkSum += rshift < 10U > (checkSum);
  }
  if (index == 11U) {
    checkSum += rshift < 11U > (checkSum);
  }
  if (index == 12U) {
    checkSum += rshift < 12U > (checkSum);
  }
  if (index == 13U) {
    checkSum += rshift < 13U > (checkSum);
  }
  if (index == 14U) {
    checkSum += rshift < 14U > (checkSum);
  }
  if (index == 15U) {
    checkSum += rshift < 15U > (checkSum);
  }
}