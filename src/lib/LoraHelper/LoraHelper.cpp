#include "URD_LoRa.h"

#ifdef ARDUINO_ARCH_ESP32
URD_LoRa::URD_LoRa(HardwareSerial* serial, int txPin, int rxPin,
                   uint8_t pinM0, uint8_t pinM1, uint8_t pinAUX,
                   float freqMHz) {
  _serial = serial;
  _pinM0 = pinM0;
  _pinM1 = pinM1;
  _pinAUX = pinAUX;
  _freqMHz = freqMHz;
  _serial->begin(9600, SERIAL_8N1, rxPin, txPin);
  _lora = new LoRa_E32(_serial, pinM0, pinM1, pinAUX);
}
#else
URD_LoRa::URD_LoRa(HardwareSerial* serial,
                   uint8_t pinM0, uint8_t pinM1, uint8_t pinAUX,
                   float freqMHz) {
  _serial = serial;
  _pinM0 = pinM0;
  _pinM1 = pinM1;
  _pinAUX = pinAUX;
  _freqMHz = freqMHz;
  _lora = new LoRa_E32(_serial, pinM0, pinM1, pinAUX);
}
#endif


void URD_LoRa::begin() {
  _lora->begin();
}


void URD_LoRa::setFrequency(uint8_t freqMHz) {
  if ((freqMHz > URD_LORA_BASE_FREQ + URD_LORA_MAX_CHANNEL) || (freqMHz < URD_LORA_BASE_FREQ)) {
    Serial.println(F("Frequencia fora da faixa → Usando 0x37 (canal 55)"));
    _channel = 0x37;
  } else {
    _channel = freqMHz - URD_LORA_BASE_FREQ;
  }
}

void URD_LoRa::setAddress(uint8_t addH, uint8_t addL) {
  _addH = addH;
  _addL = addL;
}

void URD_LoRa::applyConfig() {
  ResponseStructContainer c = _lora->getConfiguration();
  Configuration config = *(Configuration*)c.data;

  config.ADDH = _addH;
  config.ADDL = _addL;
  config.CHAN = _channel;

  ResponseStatus rs = _lora->setConfiguration(config, WRITE_CFG_PWR_DWN_SAVE);

  Serial.println();
  Serial.println(F("------ [LoRa Config] ------"));
  Serial.print(F("Address: ")); Serial.print(_addH, HEX); Serial.print(":"); Serial.println(_addL, HEX);
  Serial.print(F("Canal (hex): 0x")); Serial.println(_channel, HEX);
  Serial.print(F("Frequência: ")); Serial.print(_freqMHz); Serial.println(F(" MHz"));
  Serial.println(F("------------------------------------"));

  if (rs.code != 1)
    Serial.println(F("Lora Config err"));

  c.close();
}


void URD_LoRa::send(const String& msg) {
  _serial->print(msg);
}

void URD_LoRa::sendln(const String& msg) {
  _serial->println(msg);
}