#if defined(NRF52_PLATFORM)
#include "NRF52Board.h"

#define  BATTERY_SAMPLES 8
#define  MV_LSB   (3000.0F / 4096.0F) // 12-bit ADC with 3.0V input range


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
#if defined(PIN_VBAT_READ)
  analogReadResolution(12);
  analogReference(AR_INTERNAL_3_0);

  #if defined(PIN_BAT_CTL)
  pinMode(PIN_BAT_CTL, OUTPUT);          // battery adc can be read only ctrl pin 6 set to high
  digitalWrite(PIN_BAT_CTL, 1);
  delay(10);
  #endif

  uint32_t raw = 0;
  for (int i = 0; i < BATTERY_SAMPLES; i++) {
    raw += analogRead(PIN_VBAT_READ);
  }

  #if defined(PIN_BAT_CTL)
  digitalWrite(PIN_BAT_CTL, 0);
  #endif

  raw = raw / BATTERY_SAMPLES;

  return (uint16_t)((float)raw * MV_LSB * 4.9);
#else
  return 0;
#endif
}

#endif
