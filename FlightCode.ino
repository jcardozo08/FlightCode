//---------------------------------------------------------INICIALIZACOES---------------------------------------------------------
#include "src/lib/config.h"

#if ((GYRO) || (MAG) || (ACCEL) || (BMP180) || (USE_BMP280))
#include <Wire.h>
#endif

#if (REINICIO)
#if defined(ARDUINO_ARCH_ESP32)
#include <Preferences.h>
Preferences prefs;
bool vooAtivo;
#endif  // ARCH ESP32
#endif  // REINICIO

#if ((BMP180) || (USE_BMP280) || (SIMULADOR))
#define NUM_LEITURAS 10
float pressaoInicial;  //Variavel global para pressao inicial
double altitudeInicial;
#endif

#if USE_GY91
#if USE_BMP280
#include "src/lib/BMP280/BMP280.h"
#define bmp Barometer
BMP280 bmp;
bool flagErrBMP = false;
float lastBMP = 0;
#endif  // USE_BMP280

#if ACCEL
#include "src/lib/MPU9250_ACCEL/MPU9250_ACCEL.h"
#define accel Accelerometer
MPU9250_ACCEL accel(16);
#endif  // ACCEL

#if GYRO
#include "src/lib/MPU9250_GYRO/MPU9250_GYRO.h"
#define gyro Gyroscope
MPU9250_GYRO gyro(2000);
#endif  // GYRO

#if MAG
#include "src/lib/AK8963/AK8963.h"
#define mag Magnetometer
AK8963 mag;
#endif  // MAG
#endif  // USE_GY91

#if (BMP180)
#include "src/lib/Barometro/BMP085.h"
BMP085 bmp;
#endif

#if (FILTROS)

#define NUM_FILTROS 3 //Definindo variavel para os filtros

#if (KALMAN)
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter pressureKalmanFilter(1, 1, 0.01);
float estimated_altitude = 0.0;
#else

#include "src/lib/Filtros/Filtros.h"
//Inicializando os filtros
FiltroMediaMovel f1(NUM_LEITURAS);
FiltroMediaMovel f2(NUM_LEITURAS);
FiltroMediaMovel f3(NUM_LEITURAS);

#endif
#endif


#if (APOGEU)
#include "src/lib/Apogeu/Apogeu.h"

//Inicializando apogeu
Apogeu apogeu(10, 0.65f);
#endif

#if (SIMULADOR)
#include "src/lib/Simulador/sim.h"
Simulador sim(BAUD_RATE, LED_PIN);
#endif  // Simulador

#if (PARAQUEDAS)
#include "src/lib/Paraquedas/Paraquedas.h"


//Inicializando paraquedas
#if (P1)
Paraquedas p1(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD1, IGN_1, HEAL_1, ALTURA_ATIVACAO_PQD1);
#endif
#if (P2)
Paraquedas p2(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD2, IGN_2, HEAL_2, ALTURA_ATIVACAO_PQD2);
#endif
#if (P3)
Paraquedas p3(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD3, IGN_3, HEAL_3, ALTURA_ATIVACAO_PQD3);
#endif
#if (P4)
Paraquedas p4(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD4, IGN_4, HEAL_4, ALTURA_ATIVACAO_PQD4);
#endif

#endif

#if (USE_DHT)
#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);
#endif  // USE_DHT

#if (LORA)

#if defined(ARDUINO_AVR_MEGA2560)
HardwareSerial& LoRa = Serial3;

#elif defined(ARDUINO_ARCH_ESP32)
HardwareSerial LoRa(1);

#else
#include <SoftwareSerial.h>
SoftwareSerial LoRa(2, 3);
#endif

unsigned long previousMillisLORA = 0;
const long intervalLORA = INTERVAL_LORA;
#endif  // LORA

#if (SD_CARD)
#include <SD.h>
#include <SPI.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
SPIClass SPI_SD(FSPI);
#endif

//Definindo SD
char nomeBaseSD[] = NOME_ARQUIVO;
char nomeSD[13];  // espaço suficiente para "joao0000.txt" + null terminator
File dataFile;
#endif

#if (GPS_ON)
#include <TinyGPSPlus.h>
TinyGPSPlus gps;
#if defined(ARDUINO_AVR_MEGA2560)
HardwareSerial& GPS = Serial1;

#elif defined(ARDUINO_ARCH_ESP32)
HardwareSerial GPS(2);

#else
#include <SoftwareSerial.h>
// SoftwareSerial GPS(4, 5);
#endif
#endif  // GPS

#if (WUF)  //Wait Until Flight
bool wuf = true;
#endif

#if (TRANSONICO)
unsigned long transStart = currentTime;
bool transonico = true;
#endif 

#if (BUZZER)
#include "src/lib/Buzzer/buzzer.h"
Buzzer BeepSistemas(BUZZ_PIN, LED_PIN, BUZZ_DELAY, BUZZ_ON, BUZZ_TONE);  // pino, pinoLed, intervaloBeeps, buzzerLigaEm HIGH/LOW, toneMode
#endif

//Inicializando variavel de tempo
unsigned long currentTime = 0;
//Inicializando altitude, temperatura e pressao
float altitude = 0;




//---------------------------------------------------------FUNCOES---------------------------------------------------------

#if (USE_BMP280 || USE_BMP180 || SIMULADOR)
float getHeight(float pressure) {
  float currentHeight = 44330.0 * (1 - pow(float(pressure) / 101325, 1 / 5.25588));
  return currentHeight;
}
#endif  // USE_BMP280

void readAll() {
  bool booleano;

#if (USE_BMP280 || BMP180)
  float temperatura;
  float pressao;
#endif

#if (SIMULADOR)
  float pressao;
  pressao = sim.loop();
  altitude = sim.getHeight(pressao);
  // if (Serial.available()) {
  //   pressao = Serial.parseFloat();
  // }

  // // altitude = 44330.0f * (1.0f - powf(pressao / pressaoInicial, 0.1903f));
  // double altitudeAtual = 44330.77 * (1.0 - pow(pressao / 101325.0, 0.19026));
  // altitude = altitudeAtual - altitudeInicial;

#endif



#if (ACCEL)
  //float aX, aY, aZ, aSqrt;
  float aX, aY, aZ;
#endif
#if (GYRO)
  float gX, gY, gZ;
#endif
#if (MAG)
  float mX, mY, mZ;
#endif

#if (BMP180)

  //Filtros
  booleano = bmp.lerTudo();
  altitude = bmp.getAltitude() - alturaInicial;
  temperatura = bmp.getTemperatura();
  pressao = bmp.getPressao();

#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (USE_BMP280)

  if (bmp) {
    temperatura = bmp.getTemperature();
    pressao = bmp.getPressure();
    altitude = getHeight(pressao) - altitudeInicial;

    if (((altitude >= HEIGHT_ERR_BMP || altitude <= -HEIGHT_ERR_BMP) && currentTime <= TIME_ERR_BMP) || altitude - lastBMP >= HEIGHT_THRS_BMP) {
      flagErrBMP = true;
    }
    if (flagErrBMP) {
#if (BUZZER)
      BeepSistemas.addSystem(false);
#endif
    } else {
#if (BUZZER)
      BeepSistemas.addSystem(true);
#endif
      lastBMP = altitude;
    }
  }
#if (BUZZER)
  else {
    BeepSistemas.addSystem(false);
  }
#endif  // BUZZER
#endif  // USE_BMP280


#if (FILTROS)

#if (KALMAN)
  estimated_altitude = pressureKalmanFilter.updateEstimate(altitude);
#else
  f1.aplicarFiltro(altitude);
  f2.aplicarFiltro(f1.getMedia());
  f3.aplicarFiltro(f2.getMedia());
#endif
#endif

#if (PARAQUEDAS)
#if (SAUDE)
#if (P1)
  booleano = p1.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (P2)
  booleano = p2.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (P3)
  booleano = p3.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (P4)
  booleano = p4.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif
#endif
#endif

#if ACCEL
  if (accel) {
    aX = accel.getX();
    aY = accel.getY();
    aZ = accel.getZ();
    //aSqrt = accel.getSqrt();
#if BUZZER
    BeepSistemas.addSystem(true);
#endif
  }
#if (BUZZER)
  else {
    BeepSistemas.addSystem(false);
  }
#endif  // BUZZER
#endif

#if GYRO
  if (gyro) {
    gX = gyro.getX();
    gY = gyro.getY();
    gZ = gyro.getZ();
#if BUZZER
    BeepSistemas.addSystem(true);
#endif  // BUZZER
  }
#if (BUZZER)
  else {
    BeepSistemas.addSystem(false);
  }
#endif  // BUZZER
#endif  // GYRO


#if MAG
  if (mag) {
    mX = mag.getX();
    mY = mag.getY();
    mZ = mag.getZ();
#if BUZZER
    BeepSistemas.addSystem(true);
#endif
  }
#if (BUZZER)
  else {
    BeepSistemas.addSystem(false);
  }
#endif  // BUZZER
#endif


#if (USE_DHT)
  float tdht = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (!isnan(tdht) && !isnan(umidade)) {
#if BUZZER
    BeepSistemas.addSystem(true);
#endif
  }
#if (BUZZER)
  else {
    BeepSistemas.addSystem(false);
  }
#endif  // BUZZER
#endif  // USE_DHT

#if (GPS_ON)
  while (GPS.available()) {
    gps.encode(GPS.read());
  }
#endif  // GPS


  String dataString = "";

  dataString += String(currentTime / 1000.0, 2) + '\t';

#if (ACCEL)
  dataString += String(aX, 2) + '\t';
  dataString += String(aY, 2) + '\t';
  dataString += String(aZ, 2) + '\t';
#endif

#if (GYRO)
  dataString += String(gX, 2) + '\t';
  dataString += String(gY, 2) + '\t';
  dataString += String(gZ, 2) + '\t';
#endif

#if (MAG)
  dataString += String(mX, 2) + '\t';
  dataString += String(mY, 2) + '\t';
  dataString += String(mZ, 2) + '\t';
#endif

#if (USE_DHT)
  dataString += String(tdht, 2) + '\t';
  dataString += String(umidade, 2) + '\t';
#endif

#if (USE_BMP280)
  dataString += String(temperatura, 2) + '\t';
  dataString += String(pressao, 2) + '\t';
  dataString += String(altitude, 2) + '\t';
#endif

#if (SIMULADOR)
  dataString += sim.getDataString();

  // dataString += String(pressao, 2) + '\t';
  // dataString += String(altitude, 2) + '\t';
#endif

#if (WUF)
  dataString += String(wuf) + '\t';
#endif

#if (TRANSONICO)
  dataString += String(transonico) + '\t';
#endif

#if (GPS_ON)
  if (gps.location.isValid()) {
    dataString += String(gps.location.lat(), 6) + "\t";  // Latitude
    dataString += String(gps.location.lng(), 6) + "\t";  // Longitude
  } else {
    dataString += "0\t0\t";  // caso não tenha fixo
  }

  if (gps.altitude.isValid()) {
    dataString += String(gps.altitude.meters(), 2) + "\t";  // Altitude em metros
  }

  if (gps.speed.isValid()) {
    dataString += String(gps.speed.kmph(), 2) + "\t";  // Velocidade em km/h
  }

  if (gps.satellites.isValid()) {
    dataString += String(gps.satellites.value()) + "\t";  // Nº de satélites em uso
  }

#endif  // GPS

// --- SD CARD PRINT ---
#if (SD_CARD)
  dataFile = SD.open(nomeSD, FILE_WRITE);
  if (dataFile) {
#if (LED)
    digitalWrite(LED_PIN, HIGH);
#endif
    dataFile.print(dataString);
    dataFile.close();
#if (LED)
    digitalWrite(LED_PIN, LOW);
#endif
#if (BUZZER)
    BeepSistemas.addSystem(true);
#endif
  } else {
#if SERIAL_PRINT
    // Serial.print(F("Error opening "));
#endif
#if (BUZZER)
    BeepSistemas.addSystem(false);
#endif  // BUZZER
  }
#endif

#if (APOGEU)
  dataString += String(apogeu.getEstaDescendo()) + '\t';
  dataString += String(apogeu.getApogeuHeight(), 2) + '\t';
  dataString += String(apogeu.getApogeuTime() / 1000.0, 2) + '\t';
  dataString += String(apogeu.getMaxAltitude(), 2) + '\t';
#endif

#if (PARAQUEDAS && P1)
  dataString += String(p1.getData()) + '\t';
#if (SAUDE)
  dataString += String(p1.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && P2)
  dataString += String(p2.getData()) + '\t';
#if (SAUDE)
  dataString += String(p2.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && P3)
  dataString += String(p3.getData()) + '\t';
#if (SAUDE)
  dataString += String(p3.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && P4)
  dataString += String(p4.getData()) + '\t';
#if (SAUDE)
  dataString += String(p4.getInfo()) + '\t';
#endif  // SAUDE
#endif

// --- SERIAL PRINT ---
#if SERIAL_PRINT
  Serial.println(dataString);
#endif


#if (LORA)
  String dataStringSend = String(currentTime / 1000.0, 2) + '\t';
#if (GPS_ON)
  if (gps.location.isValid()) {
    dataStringSend += String(gps.location.lat(), 6) + "\t";
    dataStringSend += String(gps.location.lng(), 6) + "\t";
  } else {
    dataStringSend += "0\t0\t";
  }
#else
  dataStringSend += "0\t0\t";
#endif

#if (USE_BMP280)
  dataStringSend += String(altitude, 2) + "\t";
#endif  // USE_BMP280

#if (APOGEU)
  dataStringSend += String(apogeu.getMaxAltitude(), 2) + "\t";
#endif  // APOGEU

#if (PARAQUEDAS)
#if (P1)
  dataStringSend += String(p1.getPqdHeight(), 2) + "\t";
  // dataStringSend += String(p1.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (P2)
  dataStringSend += String(p2.getPqdHeight(), 2) + "\t";
  // dataStringSend += String(p2.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (P3)
  dataStringSend += String(p3.getPqdHeight(), 2) + "\t";
  // dataStringSend += String(p3.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (P4)
  dataStringSend += String(p4.getPqdHeight(), 2) + "\t";
  // dataStringSend += String(p4.getPqdTime() / 1000.0, 2) + "\t";
#endif
#else
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
#endif

#if (QUATERNIONS)
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
#else
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
  dataStringSend += "0\t0\t";
#endif


  if (currentTime - previousMillisLORA >= intervalLORA) {
    LoRa.println(dataStringSend);
    previousMillisLORA = currentTime;
  }
#endif
}

void doAll() {
#if (APOGEU)

#if (FILTROS)

#if (KALMAN)
  float alturaReal = estimated_altitude;
#else
  float alturaReal = f3.getMedia();
#endif

#else
  float alturaReal = altitude;
#endif



  //Max altitude
  apogeu.maxAltitude(alturaReal);
  //Apogeu
  apogeu.deteccaoApogeu(alturaReal, 0);  //0 para deteccao de apogeu comum || 1 para deteccao de apogeu personalizada

  //Paraquedas 1
#if (P1)
  p1.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //p1.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

  //Paraquedas 2
#if (P2)
  p2.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //p2.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

//Paraquedas 3
#if (P3)
  p3.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //p3.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

//Paraquedas 4
#if (P4)
  p4.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //p4.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

#endif
}

#if (LORA)
void setLoraFreq(float freqMHz, int pinM0 = M0_LORA_PIN, int pinM1 = M1_LORA_PIN) {
  uint8_t HEAD = 0xC2;
  uint8_t ADDH = 0x00;
  uint8_t ADDL = 0x01;
  uint8_t SPEED = 0x19;
  uint8_t OPTION_0 = 0x00;
  uint8_t OPTION_1 = 0x00;
  uint8_t CHAN = 0;

  // Arredondamento da frequência para canal inteiro
  uint16_t freqRounded = round(freqMHz);

  // Banda de 433 MHz
  if (freqRounded >= 410 && freqRounded <= 441) {
    CHAN = freqRounded - 410;
  }
  // Banda de 900 MHz
  else if (freqRounded >= 862 && freqRounded <= 930) {
    CHAN = freqRounded - 862;
  } else {
    Serial.print("Frequência inválida: ");
    Serial.println(freqMHz);
    Serial.println("Use 410-441 MHz (433) ou 862-930 MHz (900)");
    return;
  }

  // Entra no modo de configuração
  digitalWrite(pinM0, HIGH);
  digitalWrite(pinM1, HIGH);
  delay(50);

  LoRa.write(HEAD);
  LoRa.write(ADDH);
  LoRa.write(ADDL);
  LoRa.write(SPEED);
  LoRa.write(CHAN);  // Frequência arredondada
  LoRa.write(OPTION_0);
  LoRa.write(OPTION_1);

  LoRa.flush();
  delay(50);

  // Retorna ao modo de operação normal
  digitalWrite(pinM0, LOW);
  digitalWrite(pinM1, LOW);
  delay(50);

  Serial.print("LoRa configurado para ");
  Serial.print((CHAN <= 31) ? (410 + CHAN) : (862 + CHAN));
  Serial.println(" MHz");
}

#endif  // LORA

#if BUZZER
void beep(int dur, int pause, bool toneMode) {
  if (toneMode) {
    tone(BUZZ_PIN, 1000);
#if (LED)
    digitalWrite(LED_PIN, HIGH);
#endif
    delay(dur);
    noTone(BUZZ_PIN);
#if (LED)
    digitalWrite(LED_PIN, LOW);
#endif
    delay(pause);
  } else {
    digitalWrite(BUZZ_PIN, LOW);
#if (LED)
    digitalWrite(LED_PIN, HIGH);
#endif
    delay(dur);
    digitalWrite(BUZZ_PIN, HIGH);
#if (LED)
    digitalWrite(LED_PIN, LOW);
#endif
    delay(pause);
  }
}

void beepStatusSequence(bool* status, int tamanho, bool Tone) {
  for (int i = 0; i < tamanho; i++) {
    beep(100, 200, Tone);  // . curto
  }
  delay(600);
  for (int i = 0; i < tamanho; i++) {
    if (status[i]) {
      beep(100, 200, Tone);  // . curto
    } else {
      beep(300, 200, Tone);  // - longo
    }
  }
}


#endif

//---------------------------------------------------------SETUP---------------------------------------------------------

void setup() {
  Serial.begin(115200);

#if (LORA)
#if defined(ARDUINO_ARCH_ESP32)
  LoRa.begin(LORA_BAUD, SERIAL_8N1, LORA_TX, LORA_RX);
#else
  LoRa.begin(LORA_BAUD);
#endif

  pinMode(M0_LORA_PIN, OUTPUT);
  pinMode(M1_LORA_PIN, OUTPUT);
  pinMode(AUX_LORA_PIN, INPUT);

  // setLoraFreq(FREQ);
  digitalWrite(M0_LORA_PIN, LOW);
  digitalWrite(M1_LORA_PIN, LOW);
#endif  // LORA


#if (GPS_ON)
#if defined(ARDUINO_ARCH_ESP32)
  GPS.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
#else
  GPS.begin(GPS_BAUD);
#endif
#endif  // GPS


#if ((GYRO) || (MAG) || (ACCEL) || (BMP180) || (USE_BMP280))
#if defined(ARDUINO_ARCH_ESP32)
  Wire.begin(SDA_PIN, SCL_PIN);
#else
  Wire.begin();
#endif
#endif

#if CAM
  pinMode(CAM_PIN, OUTPUT);
#endif  // CAM

#if BUZZER
  bool contadorBuzz[N_SISTEMAS];
  int indexSistemaBuzz = 0;
#endif

// Inicializando BMP
#if (BMP180)
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
#endif

#if (USE_BMP280)
  bmp.begin();
  if (bmp) {
    Serial.println(F("BMP280 ok"));
#if LORA
    LoRa.println(F("BMP280 ok"));
#endif  // LORA
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  } else {
    Serial.println(F("BMP280 err"));
#if LORA
    LoRa.println(F("BMP280 err"));
#endif  // LORA
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif
#endif  //USE_BMP280

#if (ACCEL)
  accel.begin();
  if (accel) {
    Serial.println(F("Accel ok"));
#if LORA
    LoRa.println(F("Accel ok"));
#endif  // LORA
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  } else {
    Serial.println(F("Accel err"));
#if LORA
    LoRa.println(F("Accel err"));
#endif  // LORA
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif
#endif  // ACCEL

#if (GYRO)
  gyro.begin();
  if (gyro) {
    Serial.println(F("Gyro ok"));
#if LORA
    LoRa.println(F("Gyro ok"));
#endif
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  } else {
    Serial.println(F("Gyro err"));
#if LORA
    LoRa.println(F("Gyro err"));
#endif
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif
#endif  // GYRO

#if (MAG)
  mag.begin();
  if (mag) {
    Serial.println(F("Mag ok"));
#if LORA
    LoRa.println(F("Mag ok"));
#endif
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  } else {
    Serial.println(F("Mag err"));
#if LORA
    LoRa.println(F("Mag err"));
#endif
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif
#endif  // MAG

#if (USE_DHT)
  dht.begin();

  currentTime = millis();
  bool dht_ok = false;

  while (millis() - currentTime < 1500) {
    if (!isnan(dht.readTemperature()) && !isnan(dht.readHumidity())) {
      dht_ok = true;
      break;
    }
  }

  if (dht_ok) {
    Serial.println(F("DHT ok"));
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  } else {
    Serial.println(F("DHT err"));
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif
#endif  // USE_DHT


// Inicializando SD
#if (SD_CARD)

#if defined(CONFIG_IDF_TARGET_ESP32S3)
  SPI_SD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);  // inicia os pinos

  if (!SD.begin(SD_CS, SPI_SD, 40000000)) {
#else
  if (!SD.begin(SD_CS)) {
#endif

    Serial.println(F("SD card err"));
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = false;
#endif

  } else {
    Serial.println(F("SD card ok"));
#if BUZZER
    contadorBuzz[indexSistemaBuzz] = true;
#endif
  }
#if BUZZER
  indexSistemaBuzz++;
#endif

  // Lógica para nome do arquivo SD
  int iSD = 0;
  while (true) {
    sprintf(nomeSD, "%s%04d.txt", nomeBaseSD, iSD);
    // criarNomeArquivoSD(iSD);
    // Serial.print("Verificando: ");
    // Serial.println(nomeSD);

    if (SD.exists(nomeSD)) {
      // Serial.print(nomeSD);
      // Serial.println(" já existe.");
    } else {
      Serial.println(nomeSD);
      break;
    }
    iSD++;
  }
#endif

#if (P1)
  p1.begin();
#endif

#if (P2)
  p2.begin();
#endif

#if (P3)
  p3.begin();
#endif

#if (P4)
  p4.begin();
#endif

#if (REINICIO)
#if defined(ARDUINO_ARCH_ESP32)
  prefs.begin("config", false);                 // abre namespace NVS
  vooAtivo = prefs.getBool("vooAtivo", false);  // recupera estado salvo
#endif                                          // ARCH ESP32
#endif

// Primeiras leituras BMP
#if (BMP180)
  float soma = 0;
  for (int i = 0; i < NUM_LEITURAS; i++) {
    bmp.lerTudo();
    soma += bmp.getAltitude();
  }
  alturaInicial = soma / NUM_LEITURAS;
#endif

#if (USE_BMP280)
  float soma = 0;
  for (short i = 0; i < 100; i++) {
    bmp.readAll();
  }
  uint8_t contagem = 0;
  for (int i = 0; i < NUM_LEITURAS; i++) {
    if (bmp) {
      soma += bmp.getPressure();
      contagem++;
    }
  }

  pressaoInicial = soma / contagem;
  altitudeInicial = 44330.77 * (1.0 - pow(pressaoInicial / 101325.0, 0.19026));

  Serial.print(F("Baro starts: "));
  Serial.println(pressaoInicial);

#if SD_CARD
  dataFile = SD.open(nomeSD, FILE_WRITE);
  if (dataFile) {
    // Cabeçalho
    dataFile.print(F("Baro starts: "));
    dataFile.println(pressaoInicial);
  }
#endif
#endif

#if (SIMULADOR)

  // delay(3000);
  // Serial.println(F("READY"));

  // float soma = 0;
  // uint8_t contagem = 0;

  // for (int i = 0; i < NUM_LEITURAS; i++) {
  //   while (!Serial.available()) {
  //     delay(1);
  //   }
  //   float valor = Serial.parseFloat();
  //   soma += valor;
  //   contagem++;
  // }

  // pressaoInicial = soma / contagem;
  // altitudeInicial = 44330.77 * (1.0 - pow(pressaoInicial / 101325.0, 0.19026));

  // Serial.print(pressaoInicial);
  // Serial.print(" Pa \t");
  // Serial.print(altitudeInicial);
  // Serial.println(" m");

  pressaoInicial = sim.getP0();

#if (REINICIO)
  bool valorOk = true;
  if ((pressaoInicial >= 100000 || pressaoInicial <= 85000)) {
    valorOk = false;
  }
  if (!valorOk && vooAtivo) {
#if defined(ARDUINO_ARCH_ESP32)
    pressaoInicial = prefs.getFloat("pressaoIni", 0);  // testar
    altitudeInicial = getHeight(pressaoInicial);
    sim.restart(pressaoInicial, altitudeInicial);
#endif  // ARCH ESP32
    Serial.println(F("RESTART PROTECTION"));
  } else {
    vooAtivo = false;
#if defined(ARDUINO_ARCH_ESP32)
    prefs.putFloat("pressaoIni", pressaoInicial);
    prefs.putBool("vooAtivo", vooAtivo);
#endif  // ARCH ESP32
    Serial.println(F("NO RESTART PROTECTION"));
  }
#endif  // REINICIO
  Serial.print("Voo Ativo: ");
  Serial.println(vooAtivo);
  Serial.print("Pressao Ok: ");
  Serial.println(valorOk);
  Serial.print(F("Simulador - Pressao inicial / Altura inicial: "));
  Serial.print(pressaoInicial);
  Serial.print(" Pa \t");
  Serial.print(altitudeInicial);
  Serial.println(" m");

#endif

#if WUF
  Serial.print(F("WUF HEIGHT: "));
  Serial.println(WUF_HEIGHT);
#endif


#if BUZZER
  int sistemasOK = 0;
  for (int i = 0; i < indexSistemaBuzz; i++) {
    if (contadorBuzz[i]) sistemasOK++;
  }
#endif


#if BUZZER
  Serial.print(F("Sistemas ativos: "));
  Serial.println(indexSistemaBuzz);
  Serial.print(F("Sistemas ok: "));
  Serial.println(sistemasOK);
#endif

  String header = "";

  header += "tempo_s\t";

#if (USE_BMP280)
  header += "temp_C\t";
  header += "pressao_Pa\t";
  header += "alt_m\t";
#endif

#if (SIMULADOR)
  // header += "pressao_hPa\t";
  // header += "alt_m\t";
  header = sim.getHeaderString();
#endif

#if (WUF)
  header += "wuf\t";
#endif

#if (TRANSONICO)
  header += "transonic\t";
#endif

#if (APOGEU)
  header += "descendo\t";
  header += "altApogee_m\t";
  header += "tApogee_s\t";
  header += "altMax_m\t";
#endif

#if (USE_DHT)
  header += "tdht_C\t";
  header += "umidade_%\t";
#endif

#if (ACCEL)
  header += "accX_g\t";
  header += "accY_g\t";
  header += "accZ_g\t";
#endif

#if (GYRO)
  header += "gyroX_dps\t";
  header += "gyroY_dps\t";
  header += "gyroZ_dps\t";
#endif

#if (MAG)
  header += "magX_uT\t";
  header += "magY_uT\t";
  header += "magZ_uT\t";
#endif

#if (PARAQUEDAS && P1)
  header += "p1_data\t";
#if (SAUDE)
  header += "p1_info\t";
#endif
#endif

#if (PARAQUEDAS && P2)
  header += "p2_data\t";
#if (SAUDE)
  header += "p2_info\t";
#endif
#endif

#if (PARAQUEDAS && P3)
  header += "p3_data\t";
#if (SAUDE)
  header += "p3_info\t";
#endif
#endif

#if (PARAQUEDAS && P4)
  header += "p4_data\t";
#if (SAUDE)
  header += "p4_info\t";
#endif
#endif


#if (GPS_ON)
  header += "lat_deg\t";
  header += "lon_deg\t";
  header += "alt_gps_m\t";
  header += "vel_kmph";
#endif
  header += "\n";

#if (SD_CARD)
  dataFile = SD.open(nomeSD, FILE_WRITE);
  if (dataFile) {
    dataFile.println(header);
    dataFile.flush();
    dataFile.close();
  } else {
    Serial.print("Erro ao abrir ");
    Serial.println(nomeSD);
  }
#endif

  Serial.println(header);


#if (LORA)
  String sendHeader = "time_s";

#if (GPS_ON)
  sendHeader += "\tlat_deg\tlon_deg";
#endif  // GPS_ON

#if (APOGEU)
  sendHeader += "\taltMax_m";
#endif  // APOGEU

#if (USE_BMP280)
  sendHeader += "\taltAtual_m";
#endif  // USE_BMP280

#if (P1)
  sendHeader += "\tp1_height\tp1_time";
#endif
#if (P2)
  sendHeader += "\tp2_height\tp2_time";
#endif
#if (P3)
  sendHeader += "\tp3_height\tp3_time";
#endif
#if (P4)
  sendHeader += "\tp4_height\tp4_time";
#endif

  LoRa.println(sendHeader);
#endif  // LORA

#if BUZZER
  BeepSistemas.begin();
  beepStatusSequence(contadorBuzz, indexSistemaBuzz, BUZZ_TONE);
#endif

#if (WUF)
  while (((WUF_HEIGHT >= altitude) && (altitude >= -1 * WUF_HEIGHT))
#if (USE_BMP280)
         || (flagErrBMP)
#endif
  ) {
    currentTime = millis();
#if (USE_BMP280)
    if (flagErrBMP) {
      altitude = lastBMP;
      flagErrBMP = false;
    }
#endif
    readAll();
#if (BUZZER)
    BeepSistemas.beep(currentTime);
#endif
    // writeAll();
  }
  wuf = false;
#if (REINICIO)
  vooAtivo = true;
  prefs.putBool("vooAtivo", vooAtivo);
#endif  // REINICIO
#if CAM
  digitalWrite(CAM_PIN, HIGH);
#endif  // CAM
#endif  // WUF
#if (TRANSONICO)
transStart = currentTime;
while (currentTime - transStart < TIMER_TRANS) {
    currentTime = millis();
  readAll();
#if (BUZZER)
    BeepSistemas.beep(currentTime);
#endif
}
transonico = false;
#endif
}

//---------------------------------------------------------LOOP---------------------------------------------------------

void loop() {
  currentTime = millis();

  readAll();

  doAll();

  // writeAll();

#if (BUZZER)
  BeepSistemas.beep(currentTime);
#endif
}