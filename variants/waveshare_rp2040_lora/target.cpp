#include "target.h"

#include <Arduino.h>
#include <helpers/ArduinoHelpers.h>
#include <helpers/sensors/EnvironmentSensorManager.h>

WaveshareBoard board;

RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, SPI1);
WRAPPER_CLASS radio_driver(radio, board);

VolatileRTCClock fallback_clock;
AutoDiscoverRTCClock rtc_clock(fallback_clock);
#ifdef ENV_INCLUDE_INA219
  EnvironmentSensorManager sensors;
#else
  SensorManager sensors;
#endif

#ifndef LORA_CR
#define LORA_CR 5
#endif

bool radio_init() {
  rtc_clock.begin(Wire);

#ifdef SX126X_DIO3_TCXO_VOLTAGE
  float tcxo = SX126X_DIO3_TCXO_VOLTAGE;
#else
  float tcxo = 1.6f;
#endif

  SPI1.setSCK(P_LORA_SCLK);
  SPI1.setTX(P_LORA_MOSI);
  SPI1.setRX(P_LORA_MISO);

  pinMode(P_LORA_NSS, OUTPUT);
  digitalWrite(P_LORA_NSS, HIGH);

  SPI1.begin(false);

  int status = radio.begin(LORA_FREQ, LORA_BW, LORA_SF, LORA_CR, RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
                           LORA_TX_POWER, 8, tcxo);

  if (status != RADIOLIB_ERR_NONE) {
    Serial.print("ERROR: radio init failed: ");
    Serial.println(status);
    return false; // fail
  }

  radio.setCRC(1);

#ifdef SX126X_CURRENT_LIMIT
  radio.setCurrentLimit(SX126X_CURRENT_LIMIT);
#endif

#ifdef SX126X_DIO2_AS_RF_SWITCH
  radio.setDio2AsRfSwitch(SX126X_DIO2_AS_RF_SWITCH);
#endif

#ifdef SX126X_RX_BOOSTED_GAIN
  radio.setRxBoostedGainMode(SX126X_RX_BOOSTED_GAIN);
#endif

  return true; // success
}

uint32_t radio_get_rng_seed() {
  return radio.random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio.setFrequency(freq);
  radio.setSpreadingFactor(sf);
  radio.setBandwidth(bw);
  radio.setCodingRate(cr);
}

void radio_set_tx_power(uint8_t dbm) {
  radio.setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng); // create new random identity
}
