#pragma once

#include <MeshCore.h>
#include <Arduino.h>


class NRF52Board : public mesh::MainBoard {
public:
  float getMCUTemperature() override;
  uint16_t getBattMilliVolts() override;

  void loop() override {}
  #if defined(P_LORA_TX_LED)
    void onBeforeTransmit() override { digitalWrite(P_LORA_TX_LED, HIGH); }
    void onAfterTransmit() override { digitalWrite(P_LORA_TX_LED, LOW); }
  #else
    void onBeforeTransmit() override {}
    void onAfterTransmit() override {}
  #endif
};
