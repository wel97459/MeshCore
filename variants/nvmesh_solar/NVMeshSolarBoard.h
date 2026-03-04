#pragma once

#include <Arduino.h>
#include <MeshCore.h>
#include <helpers/NRF52Board.h>

// LoRa radio module pins for Heltec T114
#define P_LORA_DIO_1             20
#define P_LORA_NSS               24
#define P_LORA_RESET             25
#define P_LORA_BUSY              17
#define P_LORA_SCLK              19
#define P_LORA_MISO              23
#define P_LORA_MOSI              22

#define SX126X_DIO2_AS_RF_SWITCH true
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

class NVMeshSolarBoard : public NRF52Board {
public:
  NVMeshSolarBoard() : NRF52Board("NVMESH_SOLAR") {}
  void begin();
  void loop();

  const char *getManufacturerName() const override { return "nvme.sh Solar"; }
};
