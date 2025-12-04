#pragma once

#include <MeshCore.h>
#include <Arduino.h>

#if defined(NRF52_PLATFORM)
// built-ins
#define  BATTERY_SAMPLES 8
#define  ADC_MULTIPLIER   (3 * 1.73 * 1.187 * 1000)

class NRF52Board : public mesh::MainBoard {
public:
  float getMCUTemperature() override;
  uint16_t getBattMilliVolts() override;

  void loop() override {}
  #ifdef P_LORA_TX_LED
    void onBeforeTransmit() override { digitalWrite(P_LORA_TX_LED, HIGH); }
    void onAfterTransmit() override { digitalWrite(P_LORA_TX_LED, LOW); }
  #else
    void onBeforeTransmit() override {}
    void onAfterTransmit() override {}
  #endif
};

#endif