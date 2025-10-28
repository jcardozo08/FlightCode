//------------------------------------------------- SISTEMA DE CONFIGURAÇÃO -------------------------------------------------//
// Ativa/desativa sensores e módulos do sistema embarcado. Cada macro controla a inclusão de uma funcionalidade.

//------------------------------ SENSORES PRINCIPAIS ------------------------------//
#define SENSORES 1  // Ativa sensores em geral

#define USE_GY91 ((SENSORES) && 1)  // Módulo GY-91 (accel, gyro, mag, bmp280)
#define USE_BMP280 ((USE_GY91) && 1)  // Barômetro BMP280 (com o modulo GY91)
#define ACCEL ((USE_GY91) && 1)     // Acelerômetro GY91
#define GYRO ((USE_GY91) && 1)      // Giroscópio GY91
#define MAG ((USE_GY91) && 0)       // Magnetômetro GY91

#define USE_DHT ((SENSORES) && 0)   // Sensor de temperatura e umidade (DHT)

#define BMP180 ((SENSORES) && 0)      // Barômetro BMP180

//------------------------------ FUNÇÕES BASEADAS NO BARÔMETRO ------------------------------//
// #define VVERTICAL ((BMP180 || USE_GY91) && 0)  // Cálculo de velocidade vertical
#define FILTROS ((BMP180 || USE_GY91 || SIMULADOR) && 1)    // Filtros aplicados aos dados do barômetro
  #define KALMAN (FILTROS && 1)   // Altera entre suavização por filtro de kalman ou media movel

#define APOGEU ((BMP180 || USE_GY91 || SIMULADOR) && 1)     // Detecção de apogeu
#define PARAQUEDAS (APOGEU && 1)               // Sistema de ejeção de paraquedas

// Subfunções do sistema de paraquedas
#define SAUDE (PARAQUEDAS && 0)  // Verificação de saúde do sistema
#define P1 (PARAQUEDAS && 1)
#define P2 (PARAQUEDAS && 1)
#define P3 (PARAQUEDAS && 1)
#define P4 (PARAQUEDAS && 1)

//------------------------------ ARMAZENAMENTO ------------------------------//
#define SD_CARD 1           // Habilita módulo de cartão SD
#define NOME_ARQUIVO "RD"  // Prefixo do nome do arquivo de log

//------------------------------ MODULOS ------------------------------//
#define LORA 1      // Comunicação via LoRa
#define GPS_ON 1    // GPS serial
#define QUATERNIONS 1 // Liga quaternions
#define CAM 0       // Ativa pino de gravação da camera ao iniciar voo
#define SIGNAL 0 // Ativa IO para sinal

//------------------------------ CONTROLE DE VOO ------------------------------//
#define WUF 1  // Espera de condição de voo antes de liberar ações pirotecnicas
#define TRANSONICO 0 // Timer de voo transonico
#define REINICIO 0 // Proteção contra reinicio durante voo

//------------------------------ SINALIZAÇÕES ------------------------------//
#define LED 1
#define BUZZER 1  // Habilita buzzer
//#define APITOS          (1 && BUZZER)                // Habilita sequência de apitos (ex: armado, erro, ok)
//#define TimedPinController 0               // Biblioteca para controle de pinos baseado em tempo (sem delay)


//------------------------------ DEBUG & MONITORAMENTO ------------------------------//
#define SERIAL_PRINT 1  // Exibe informações na porta serial
#define MARCADORES 0    // Marcadores para visualizacao no grafico
#define DEBUG 0  // Ativa prints extras de debug
#define TESTE_PQD 0


//------------------------------ TESTES ------------------------------//
#define TESTE_ELEVADOR   0                   // Modo de teste em elevadores
#define SIMULADOR  (!USE_BMP280 && 1)                   // Modo simulação de voo
// #define TESTE_ASPIRADOR  0                   // Modo de teste com aspiradores

#define N_SISTEMAS (USE_BMP280 + ACCEL + GYRO + MAG + USE_DHT + SD_CARD)
#define BAUD_RATE 115200

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CONFIGS E PINOS  -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-//

// PINOS I2C
#if defined(ARDUINO_ARCH_ESP32)
#define SDA_PIN 21
#define SCL_PIN 22
#endif

// CARTAO SD
#if (SD_CARD)
#if defined(CONFIG_IDF_TARGET_ESP32S3)
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 
#elif defined(CONFIG_IDF_TARGET_ESP32)
#define SD_CS 5
#else
#define SD_CS 53
#endif
#endif

// BUZZER
#if (BUZZER)
#if defined(ARDUINO_ARCH_ESP32)
#define BUZZ_PIN 27
#elif defined(ARDUINO_AVR_MEGA2560)
#define BUZZ_PIN 22
#endif

#define BUZZ_DELAY 10000
#define BUZZ_ON LOW
#define BUZZ_TONE true
#endif

// WUF
#if (WUF)
#if (TESTE_ELEVADOR)
#define WUF_HEIGHT 1
#else
#define WUF_HEIGHT 50
#endif
#endif

// TRANSONICO
#if (TRANSONICO)
#if (TESTE_ELEVADOR)
#define TIMER_TRANS 3000 // ms
#else
#define TIMER_TRANS 10000 // ms
#endif
#endif

// BAROMETRO
#if (USE_BMP280)
#define HEIGHT_ERR_BMP 100
#define TIME_ERR_BMP 20000
#define HEIGHT_THRS_BMP 300
#endif

// DHT
#if (USE_DHT)
#if defined(ARDUINO_ARCH_ESP32)
#define DHTPIN 14       
#elif defined(ARDUINO_AVR_MEGA2560)
#define DHTPIN 3       
#endif

#define DHTTYPE DHT22  // Tipo do sensor
#endif

// PINO ADC EXTRA (SIGNAL)
#if (SIGNAL)
#if defined(ARDUINO_ARCH_ESP32)
#define SIGNAL 34             
#endif
#endif

// LED
#if (LED)
#if defined(ARDUINO_ARCH_ESP32)
#define LED_PIN 2
#elif defined(ARDUINO_AVR_MEGA2560)
#define LED_PIN 9
#else
#define LED_PIN 13
#endif
#endif


// CAM
#if (CAM)
#define CAMPIN 7
#endif

// LORA
#if (LORA)
#if defined(ARDUINO_ARCH_ESP32)
#define M0_LORA_PIN 26
#define M1_LORA_PIN 25
#define AUX_LORA_PIN 36
#define LORA_RX 33 // p/ ESP32
#define LORA_TX 32 // p/ ESP32
#else
#define M0_LORA_PIN 12
#define M1_LORA_PIN 11
#define AUX_LORA_PIN A8
#endif

#define LORA_BAUD 9600
#define FREQ 917 // MHz
#define INTERVAL_LORA 2000
#endif 

// GPS
#if (GPS)
#if defined(ARDUINO_ARCH_ESP32)
#define GPS_RX 34        
#define GPS_TX 12  
#endif
#define GPS_BAUD 9600
#endif

// PQDs
#if (PARAQUEDAS)
  #define TEMPO_LIGADO_PQD 1000 // ms

  #define TEMPO_DELAY_PQD1 0 // ms
  #define TEMPO_DELAY_PQD2 5000 // ms
  #define TEMPO_DELAY_PQD3 0 // ms
  #define TEMPO_DELAY_PQD4 0 // ms

#if (TESTE_ELEVADOR)
  #define ALTURA_ATIVACAO_PQD1 0 // Apogeu
  #define ALTURA_ATIVACAO_PQD2 0 // Apogeu
  #define ALTURA_ATIVACAO_PQD3 25
  #define ALTURA_ATIVACAO_PQD4 20
#else
  #define ALTURA_ATIVACAO_PQD1 0 // Apogeu
  #define ALTURA_ATIVACAO_PQD2 0 // Apogeu
  #define ALTURA_ATIVACAO_PQD3 450
  #define ALTURA_ATIVACAO_PQD4 400
#endif

#if defined(ARDUINO_ARCH_ESP32)
#if (P1)
  #define IGN_1 4 /*act1*/
  #define HEAL_1 39 /*health 1*/
#endif

#if (P2)
  #define IGN_2 2 /*act2*/
  #define HEAL_2 39 /*health 2*/
#endif

#if (P3)
  #define IGN_3 15 /*act3*/
  #define HEAL_3 39 /*health 3*/
#endif

#if (P4)
  #define IGN_4 13 /*act4*/
  #define HEAL_4 39 /*health 4*/
#endif


#elif defined(ARDUINO_AVR_MEGA2560)
#if (P1)
  #define IGN_1 A0 /*act1*/
  #define HEAL_1 A1 /*health 1*/
#endif

#if (P2)
  #define IGN_2 A2 /*act2*/
  #define HEAL_2 A3 /*health 2*/
#endif

#if (P3)
  #define IGN_3 A4 /*act3*/
  #define HEAL_3 A5 /*health 3*/
#endif

#if (P4)
  #define IGN_4 A6 /*act4*/
  #define HEAL_4 A7 /*health 4*/
#endif

#endif 
#endif


