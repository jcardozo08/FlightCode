#ifndef URD_LORA_H
#define URD_LORA_H



#if defined(FREQUENCY_900) + defined(FREQUENCY_868) + defined(FREQUENCY_433) > 1
  #error "⚠️ Apenas uma frequência deve ser definida! Use #define FREQUENCY_900 ou 868 ou 433 antes do include."
#elif defined(FREQUENCY_900)
  #define URD_LORA_MODULE "E32-900"
  #define URD_LORA_BASE_FREQ 862.0
  #define URD_LORA_MAX_CHANNEL 69
#elif defined(FREQUENCY_868)
  #define URD_LORA_MODULE "E32-868"
  #define URD_LORA_BASE_FREQ 850.0
  #define URD_LORA_MAX_CHANNEL 30
#elif defined(FREQUENCY_433)
  #define URD_LORA_MODULE "E32-433"
  #define URD_LORA_BASE_FREQ 410.0
  #define URD_LORA_MAX_CHANNEL 31
#else
  #error "⚠️ Defina a frequência do módulo antes de incluir: #define FREQUENCY_900 (ou 868/433)"
#endif

#include "Arduino.h"
#include "LoRa_E32.h"

class URD_LoRa {
  private:
    LoRa_E32* _lora;
    HardwareSerial* _serial;
    uint8_t _pinM0, _pinM1, _pinAUX;
    float _freqMHz = 0.0;
    uint8_t _channel = 0x37; // canal padrão ≈ 917 MHz
    uint8_t _addH = 0x00;
    uint8_t _addL = 0x17;

  public:
    #ifdef ARDUINO_ARCH_ESP32
      URD_LoRa(HardwareSerial* serial, int txPin, int rxPin,
               uint8_t pinM0, uint8_t pinM1, uint8_t pinAUX,
               float freqMHz);
    #else
      URD_LoRa(HardwareSerial* serial,
               uint8_t pinM0, uint8_t pinM1, uint8_t pinAUX,
               float freqMHz);
    #endif

    void begin();
    void setFrequency(uint8_t freqMHz);
    void setAddress(uint8_t addH, uint8_t addL);
    void applyConfig();
    void send(const String& msg);
    void sendln(const String& msg);

};

#endif
