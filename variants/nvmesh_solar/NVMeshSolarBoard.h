#pragma once

#include <MeshCore.h>
#include <Arduino.h>

#include <nrf.h>
#include <nrf_temp.h>
#include "nrf_gpio.h"
#include "nrf_gpiote.h"

#include <helpers/NRF52Board.h>
// LoRa radio module pins for Heltec T114
#define  P_LORA_DIO_1     20
#define  P_LORA_NSS       24
#define  P_LORA_RESET     25
#define  P_LORA_BUSY      17
#define  P_LORA_SCLK      19
#define  P_LORA_MISO      23
#define  P_LORA_MOSI      22

#define SX126X_DIO2_AS_RF_SWITCH  true
#define SX126X_DIO3_TCXO_VOLTAGE   1.8


class NVMeshSolarBoard : public NRF52Board {
protected:
  uint8_t startup_reason;

public:
  void begin();
  void loop();
  uint8_t getStartupReason() const override { return startup_reason; }

  uint16_t getBattMilliVolts() override {
    #ifdef HELTEC_MESH_SOLAR
      return meshSolarGetBattVoltage();
    #else
      return 0;
    #endif
  }

  #ifdef P_LORA_TX_LED
    void onBeforeTransmit() override { digitalWrite(P_LORA_TX_LED, HIGH); }
    void onAfterTransmit() override { digitalWrite(P_LORA_TX_LED, LOW); }
  #endif

  const char* getManufacturerName() const override {
    return "Heltec Mesh Solar";
  }

  void reboot() override {
    NVIC_SystemReset();
  }

  bool startOTAUpdate(const char* id, char reply[]) override;
};
