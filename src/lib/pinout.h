#ifndef PINOUT_H
#define PINOUT_H
#include "config.h"
#ifndef NOME_PLACA
  #error "Inclua o config antes do pinout.h"
#endif

// ============================================================================
// PLACA MAIN OCTA PTH
// ============================================================================
#if defined(PLACA_MAIN_OCTA_PTH)

  // ------------------------------ CARTAO SD ------------------------------//
  #if (SD_CARD)
    #define SD_CS 53
  #endif

  // ------------------------------ BUZZER ------------------------------//
  #if (BUZZER)
    #define BUZZ_PIN 22
    #define BUZZ_TONE 1
    #define BUZZ_ON LOW
  #endif

  // ------------------------------ DHT ------------------------------//
  #if (USE_DHT)
    #define DHT_PIN 3
  #endif

  // ------------------------------ SIGNAL ------------------------------//
  #if (SIGNAL)
    // #define SIGNAL_PIN 
  #endif

  // ------------------------------ LED ------------------------------//
  #if (LED)
    #define LED_PIN 9
  #endif

  // ------------------------------ CAM ------------------------------//
  #if (CAM)
    // #define CAM_PIN
  #endif

  // ------------------------------ LORA ------------------------------//
  #if (LORA)
    #define M0_LORA_PIN 12
    #define M1_LORA_PIN 11
    #define AUX_LORA_PIN A8
  #endif

  // ------------------------------ GPS ------------------------------//
  #if (GPS_ON)
    #define GPS_RX 17
    #define GPS_TX 16
  #endif

  // ------------------------------ PARAQUEDAS / PQDs ------------------------------//
  #if (PQD_DN)
    #define IGN_1 A0
    #if (SAUDE)
      #define HEAL_1 A1
    #endif
  #endif

  #if (PQD_DB)
    #define IGN_2 A2
    #if (SAUDE)
    #define HEAL_2 A3
    #endif
  #endif

  #if (PQD_MN)
    #define IGN_3 A4
    #if (SAUDE)
      #define HEAL_3 A5
    #endif
  #endif

  #if (PQD_MB)
    #define IGN_4 A6
    #if (SAUDE)
      #define HEAL_4 A7
    #endif
  #endif

// ============================================================================
// ESSENTIALS V1
// ============================================================================
#elif defined(PLACA_ESSENTIALS_V1)

  // ------------------------------ I2C ------------------------------//
    #define SDA_PIN 21
    #define SCL_PIN 22

  // ------------------------------ CARTAO SD ------------------------------//
  #if (SD_CARD)
    #define SD_MOSI 23
    #define SD_MISO 19
    #define SD_SCK 18
    #define SD_CS 5
  #endif

  // ------------------------------ BUZZER ------------------------------//
  #if (BUZZER)
    #define BUZZ_PIN 27
    #define BUZZ_TONE 0
    #define BUZZ_ON HIGH
  #endif

  // ------------------------------ DHT ------------------------------//
  #if (USE_DHT)
    #define DHT_PIN 14
  #endif

  // ------------------------------ SIGNAL ------------------------------//
  #if (SIGNAL)
    #define SIGNAL_PIN 34
  #endif

  // ------------------------------ LED ------------------------------//
  #if (LED)
    #define LED_PIN 12
  #endif

  // ------------------------------ CAM ------------------------------//
  #if (CAM)
    #define CAM_PIN 7
  #endif

  // ------------------------------ LORA ------------------------------//
  #if (LORA)
    #define M0_LORA_PIN 26
    #define M1_LORA_PIN 25
    #define AUX_LORA_PIN 36
    #define LORA_RX 33
    #define LORA_TX 32
  #endif

  // ------------------------------ GPS ------------------------------//
  #if (GPS_ON)
    #define GPS_RX 17
    #define GPS_TX 16
  #endif

  // ------------------------------ PARAQUEDAS / PQDs ------------------------------//
  #if ((PQD_DN || PQD_DB || PQD_MN || PQD_MB))
    #warning "A placa ESSENTIALS V1 não possui suporte para paraquedas."
    #if (SAUDE)
      #warning "A funcionalidade de saúde do sistema de paraquedas também não é suportada." 
    #endif
    #error "Desative as opções de paraquedas no config.h para evitar erros de compilação."
  #endif

// ============================================================================
// PLACA MAIN SMD
// ============================================================================
#elif defined(PLACA_MAIN_SMD)

  // ------------------------------ I2C ------------------------------//
    #define SDA_PIN 42
    #define SCL_PIN 41

  // ------------------------------ CARTAO SD ------------------------------//
  #if (SD_CARD)
    #define SD_MOSI 11
    #define SD_MISO 13
    #define SD_SCK 12
    #define SD_CS 14
  #endif

  // ------------------------------ BUZZER ------------------------------//
  #if (BUZZER)
    #define BUZZ_PIN 37
    #define BUZZ_TONE 0
    #define BUZZ_ON HIGH
  #endif

  // ------------------------------ DHT ------------------------------//
  #if (USE_DHT)
    // #define DHT_PIN
  #endif

  // ------------------------------ SIGNAL ------------------------------//
  #if (SIGNAL)
    // #define SIGNAL_PIN
  #endif

  // ------------------------------ LED ------------------------------//
  #if (LED)
    #define LED_PIN 1
  #endif

  // ------------------------------ CAM ------------------------------//
  #if (CAM)
    // #define CAM_PIN
  #endif

  // ------------------------------ LORA ------------------------------//
  #if (LORA)
    #define M0_LORA_PIN 16
    #define M1_LORA_PIN 15
    #define AUX_LORA_PIN 8
    #define LORA_RX 17
    #define LORA_TX 18
  #endif

  // ------------------------------ GPS ------------------------------//
  #if (GPS_ON)
    #define GPS_RX 47
    #define GPS_TX 21
  #endif

  // ------------------------------ PARAQUEDAS / PQDs ------------------------------//
  #if (PQD_DN)
    #define IGN_1 6
    #if (SAUDE)
      #define HEAL_1 7
    #endif
  #endif

  #if (PQD_DB)
    #define IGN_2 4
    #if (SAUDE)
      #define HEAL_2 5
    #endif
  #endif

  #if (PQD_MN)
    #define IGN_3 36
    #if (SAUDE)
      #define HEAL_3 9
    #endif
  #endif

  #if (PQD_MB)
    #define IGN_4 35
    #if (SAUDE)
      #define HEAL_4 3
    #endif
  #endif

// ============================================================================
// PLACA MAIN PTH
// ============================================================================
#elif defined(PLACA_MAIN_PTH)

  // ------------------------------ I2C ------------------------------//
    #define SDA_PIN 8
    #define SCL_PIN 9

  // ------------------------------ CARTAO SD ------------------------------//
  #if (SD_CARD)
    #define SD_MOSI 11
    #define SD_MISO 13
    #define SD_SCK 12
    #define SD_CS 14
  #endif

  // ------------------------------ BUZZER ------------------------------//
  #if (BUZZER)
    #define BUZZ_PIN 42
    #define BUZZ_TONE 1
    #define BUZZ_ON LOW
  #endif

  // ------------------------------ DHT ------------------------------//
  #if (USE_DHT)
    // #define DHT_PIN 
  #endif

  // ------------------------------ SIGNAL ------------------------------//
  #if (SIGNAL)
    // #define SIGNAL_PIN
  #endif

  // ------------------------------ LED ------------------------------//
  #if (LED)
    #define LED_PIN 35
  #endif

  // ------------------------------ CAM ------------------------------//
  #if (CAM)
    // #define CAM_PIN
  #endif

  // ------------------------------ LORA ------------------------------//
  #if (LORA)
    #define M0_LORA_PIN 18
    #define M1_LORA_PIN 17
    #define AUX_LORA_PIN 41
    #define LORA_RX 16
    #define LORA_TX 15
  #endif

  // ------------------------------ GPS ------------------------------//
  #if (GPS_ON)
    #define GPS_RX 40
    #define GPS_TX 39
  #endif

  // ------------------------------ PARAQUEDAS / PQDs ------------------------------//
  #if (PQD_DN)
    #define IGN_1 4
    #if (SAUDE)
      #define HEAL_1 5
    #endif
  #endif

  #if (PQD_DB)
    #define IGN_2 6
    #if (SAUDE)
      #define HEAL_2 7
    #endif
  #endif

  #if (PQD_MN)
    #define IGN_3 21
    #if (SAUDE)
      #define HEAL_3 2
    #endif
  #endif

  #if (PQD_MB)
    #define IGN_4 47
    #if (SAUDE)
      #define HEAL_4 1
    #endif
  #endif

// ============================================================================
// PLACA MAIN IC
// ============================================================================
#elif defined(PLACA_MAIN_IC)

  // ------------------------------ I2C ------------------------------//
    #define SDA_PIN 42
    #define SCL_PIN 41

  // ------------------------------ CARTAO SD ------------------------------//
  #if (SD_CARD)
    #define SD_MOSI 11
    #define SD_MISO 13
    #define SD_SCK 12
    #define SD_CS 14
  #endif

  // ------------------------------ BUZZER ------------------------------//
  #if (BUZZER)
    #define BUZZ_PIN 37
    #define BUZZ_TONE 0
    #define BUZZ_ON HIGH
  #endif

  // ------------------------------ USE_DHT ------------------------------//
  #if (USE_DHT)
    // #define DHT_PIN
  #endif

  // ------------------------------ SIGNAL ------------------------------//
  #if (SIGNAL)
    // #define SIGNAL_PIN
  #endif

  // ------------------------------ LED ------------------------------//
  #if (LED)
    #define LED_PIN 1
  #endif

  // ------------------------------ CAM ------------------------------//
  #if (CAM)
    // #define CAM_PIN
  #endif

  // ------------------------------ LORA ------------------------------//
  #if (LORA)
    #define M0_LORA_PIN 16
    #define M1_LORA_PIN 15
    #define AUX_LORA_PIN 8
    #define LORA_RX 17
    #define LORA_TX 18
  #endif

  // ------------------------------ GPS ------------------------------//
  #if (GPS_ON)
    #define GPS_RX 47
    #define GPS_TX 21
  #endif

  // ------------------------------ PARAQUEDAS / PQDs ------------------------------//
  #if (PQD_DN)
    #define IGN_1 6
    #if (SAUDE)
      #define HEAL_1 7
    #endif
  #endif

  #if (PQD_DB)
    #define IGN_2 4
    #if (SAUDE)
      #define HEAL_2 5
    #endif
  #endif

  #if (PQD_MN)
    #define IGN_3 36
    #if (SAUDE)
      #define HEAL_3 9
    #endif
  #endif

  #if (PQD_MB)
    #define IGN_4 35
    #if (SAUDE)
      #define HEAL_4 3
    #endif
  #endif

#else
  #error "Placa selecionada sem bloco correspondente no pinout.h"
#endif

#endif