#if defined(NRF52_PLATFORM)
#include "NRF52Board.h"

// The default is from the built-in temperature sensor of MCU
float NRF52Board::getMCUTemperature() {
  NRF_TEMP->TASKS_START = 1; // Start temperature measurement

  while (NRF_TEMP->EVENTS_DATARDY == 0) { } // Wait for completion
  NRF_TEMP->EVENTS_DATARDY = 0; // Clear event flag

  int32_t temp = NRF_TEMP->TEMP; // In 0.25°C units
  NRF_TEMP->TASKS_STOP = 1;

  return temp * 0.25f; // Convert to °C
}

uint16_t NRF52Board::getBattMilliVolts() {
  #ifdef PIN_VBAT_READ
  analogReadResolution(12);

  uint32_t raw = 0;
  for (int i = 0; i < BATTERY_SAMPLES; i++) {
    raw += analogRead(PIN_VBAT_READ);
  }
  raw = raw / BATTERY_SAMPLES;

  return (ADC_MULTIPLIER * raw) / 4096;
  #else
  return 0;
  #endif
}

#endif
