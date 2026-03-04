#include "NVMeshSolarBoard.h"

#include "variant.h"

#include <Arduino.h>
#include <Wire.h>
#ifdef WDT_ENABLED
#include "tpl5010.h"
static TPL5010 tpl5010;
#endif

void NVMeshSolarBoard::begin() {
  // for future use, sub-classes SHOULD call this from their begin()
  startup_reason = BD_STARTUP_NORMAL;
  NRF52Board::begin();
#ifdef HELTEC_MESH_SOLAR
  meshSolarStart();
#endif
#ifdef P_LORA_TX_LED
  pinMode(P_LORA_TX_LED, OUTPUT);
#endif
#if defined(PIN_BOARD_SDA) && defined(PIN_BOARD_SCL)
  Wire.setPins(PIN_BOARD_SDA, PIN_BOARD_SCL);
#endif

#ifdef WDT_ENABLED
  tpl5010.begin();
#endif

  Wire.begin();
}

void NVMeshSolarBoard::loop() {
#ifdef WDT_ENABLED
  tpl5010.kick_if_needed();
#endif
}
