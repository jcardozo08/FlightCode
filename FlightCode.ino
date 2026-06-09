//---------------------------------------------------------INICIALIZACOES---------------------------------------------------------
#include "src/lib/config.h"
#include "src/lib/pinout.h"

unsigned long currentTime = 0;  //Inicializando variavel de tempo
float altitude = 0;             //Inicializando altitude
int contador_linha = 1;         //Inicializando contador de linha

#if ((GYRO) || (MAG) || (ACCEL) || (BMP180) || (USE_BMP280))
#include <Wire.h>
#endif

#if (REINICIO)
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
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

#define NUM_FILTROS 3  //Definindo variavel para os filtros

#if (CALIBRATION_IMU)
#define BIAS_ACCEL_X 0.0008
#define BIAS_ACCEL_Y 0.0038
#define BIAS_ACCEL_Z -0.0023
#define SCALE_ACCEL_X 1.012696
#define SCALE_ACCEL_Y 0.999127
#define SCALE_ACCEL_Z 0.996743

#define BIAS_GYRO_X 4.572174
#define BIAS_GYRO_Y -1.737915
#define BIAS_GYRO_Z 0.417297
#endif

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

#if (KALMAN_IMU)
#include <SimpleKalmanFilter.h>
// Parâmetros: processNoise, measurementNoise, estimatedError
SimpleKalmanFilter kal_ax(2, 2, 0.01);
SimpleKalmanFilter kal_ay(2, 2, 0.01);
SimpleKalmanFilter kal_az(2, 2, 0.01);

SimpleKalmanFilter kal_gx(2, 2, 0.01);
SimpleKalmanFilter kal_gy(2, 2, 0.01);
SimpleKalmanFilter kal_gz(2, 2, 0.01);

float ax_kal = 0;
float ay_kal = 0;
float az_kal = 0;

float gx_kal = 0;
float gy_kal = 0;
float gz_kal = 0;


#if (ENABLE_3D)
#include <MahonyAHRS.h>
Mahony filter;
#define FREQ_3D 100.0f  // Frequência de atualização (Hz)
float yaw_offset = 0.0f;
bool yaw_zeroed = false;
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
#if (PQD_DN)
Paraquedas pqd_dn(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD1, IGN_1, HEAL_1, ALTURA_ATIVACAO_PQD1);
#endif
#if (PQD_DB)
Paraquedas pqd_db(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD2, IGN_2, HEAL_2, ALTURA_ATIVACAO_PQD2);
#endif
#if (PQD_MN)
Paraquedas pqd_mn(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD3, IGN_3, HEAL_3, ALTURA_ATIVACAO_PQD3);
#endif
#if (PQD_MB)
Paraquedas pqd_mb(TEMPO_LIGADO_PQD, TEMPO_DELAY_PQD4, IGN_4, HEAL_4, ALTURA_ATIVACAO_PQD4);
#endif

#endif

#if (USE_DHT)
#include "DHT.h"
DHT dht(DHT_PIN, DHTTYPE);
#endif  // USE_DHT

#if (LORA)

#if (PLACA_MAIN_OCTA_PTH)
HardwareSerial& LoRa = Serial3;

#elif defined(ARDUINO_ARCH_ESP32)
HardwareSerial LoRa(1);

#else
#include <SoftwareSerial.h>
SoftwareSerial LoRa(2, 3);
#endif
// #define FREQUENCY_900
// #include "src/lib/LoraHelper/LoraHelper.h"
// #if (PLACA_MAIN_OCTA_PTH)
// Lora_Helper LoRa(Serial3, M0_LORA_PIN, M1_LORA_PIN, AUX_LORA_PIN, FREQ);


// #elif defined(ARDUINO_ARCH_ESP32)
// Lora_Helper LoRa(LoRa(1), LORA_TX, LORA_RX, M0_LORA_PIN, M1_LORA_PIN, AUX_LORA_PIN, FREQ);


// #else
// #include <SoftwareSerial.h>
// SoftwareSerial LoRa(2, 3);
// #endif

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
#if (PLACA_MAIN_OCTA_PTH)
HardwareSerial& GPS = Serial1;

#elif defined(ARDUINO_ARCH_ESP32)
HardwareSerial GPS(2);

#else
HardwareSerial GPS(2);
// #include <SoftwareSerial.h>
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
#if (PQD_DN)
  booleano = pqd_dn.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (PQD_DB)
  booleano = pqd_db.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (PQD_MN)
  booleano = pqd_mn.getInfo();
#if (BUZZER)
  BeepSistemas.addSystem(booleano);
#endif
#endif

#if (PQD_MB)
  booleano = pqd_mb.getInfo();
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
#if (CALIBRATION_IMU)
    aX = (aX - BIAS_ACCEL_X) * SCALE_ACCEL_X;
    aY = (aY - BIAS_ACCEL_Y) * SCALE_ACCEL_Y;  // Note o sinal negativo no bias
    aZ = (aZ - BIAS_ACCEL_Z) * SCALE_ACCEL_Z;
#endif
#if (KALMAN_IMU)
    ax_kal = kal_ax.updateEstimate(aX);
    ay_kal = kal_ay.updateEstimate(aY);
    az_kal = kal_az.updateEstimate(aZ);
#endif
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
#if (CALIBRATION_IMU)
    gX = (gX - BIAS_GYRO_X);
    gY = (gY - BIAS_GYRO_Y);  // Note o sinal negativo no bias
    gZ = (gZ - BIAS_GYRO_Z);
#endif
#if (KALMAN_IMU)
    gx_kal = kal_gx.updateEstimate(gX);
    gy_kal = kal_gy.updateEstimate(gY);
    gz_kal = kal_gz.updateEstimate(gZ);
#endif


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

#if (ENABLE_3D)
  az_kal = -az_kal;
  gz_kal = -gz_kal;

  // --- Atualiza filtro Mahony ---
  filter.updateIMU(gx_kal, gy_kal, gz_kal, ax_kal, ay_kal, az_kal);

  // --- Calcula ângulos de Euler ---
  float roll = filter.getRoll();
  float pitch = filter.getPitch();
  float yaw = filter.getYaw();

  if (!yaw_zeroed) {
    yaw_offset = yaw;
    yaw_zeroed = true;
  }
  yaw -= yaw_offset;

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
    dataString += "~\t~\t";
  }

  if (gps.altitude.isValid()) {
    dataString += String(gps.altitude.meters(), 2) + "\t";  // Altitude em metros
  } else {
    dataString += "~\t";
  }

  if (gps.speed.isValid()) {
    dataString += String(gps.speed.kmph(), 2) + "\t";  // Velocidade em km/h
  } else {
    dataString += "~\t";
  }

  if (gps.satellites.isValid()) {
    dataString += String(gps.satellites.value()) + "\t";  // Nº de satélites em uso
  } else {
    dataString += "~\t";
  }

  if (gps.time.isValid()) {
    int hora = gps.time.hour();
    int minuto = gps.time.minute();

    // Corrige para UTC-3
    hora = (hora - 3 + 24) % 24;

    char dataHora[32];
    sprintf(dataHora, "%02d:%02d", hora, minuto);

    dataString += String(dataHora) + "\t";  // Adiciona à string de dados
  } else {
    dataString += "~\t~\t";
  }

  if (gps.hdop.isValid()) {
    dataString += String(gps.hdop.hdop(), 1) + "\t";
  } else {
    dataString += "~\t";
  }

#endif  // GPS

// --- SD CARD PRINT ---
#if (SD_CARD)
  bool sd_bool = false;
  dataFile = SD.open(nomeSD, FILE_APPEND);
  if (dataFile) {
    sd_bool = true;
#if (LED)
    digitalWrite(LED_PIN, HIGH);
#endif
    dataFile.println(dataString);
    dataFile.close();
#if (LED)
    digitalWrite(LED_PIN, LOW);
#endif
#if (BUZZER)
    BeepSistemas.addSystem(true);
#endif
  } else {
#if SERIAL_PRINT
    Serial.print(F("Error opening "));
    Serial.print(nomeSD);
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

#if (PARAQUEDAS && PQD_DN)
  dataString += String(pqd_dn.getData()) + '\t';
#if (SAUDE)
  dataString += String(pqd_dn.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && PQD_DB)
  dataString += String(pqd_db.getData()) + '\t';
#if (SAUDE)
  dataString += String(pqd_db.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && PQD_MN)
  dataString += String(pqd_mn.getData()) + '\t';
#if (SAUDE)
  dataString += String(pqd_mn.getInfo()) + '\t';
#endif  // SAUDE
#endif

#if (PARAQUEDAS && PQD_MB)
  dataString += String(pqd_mb.getData()) + '\t';
#if (SAUDE)
  dataString += String(pqd_mb.getInfo()) + '\t';
#endif  // SAUDE
#endif

// --- SERIAL PRINT ---
#if SERIAL_PRINT
  Serial.println(dataString);
#endif


#if (LORA)

  if (currentTime - previousMillisLORA >= intervalLORA) {

    String dataStringSend = "L:" + String(contador_linha) + "\t";
    contador_linha += 1;
    dataStringSend += "T:" + String(currentTime / 1000.0, 2) + "\t";

#if (GPS_ON)
    if (gps.location.isValid()) {
      dataStringSend += "LAT:" + String(gps.location.lat(), 6) + "\t";
      dataStringSend += "LON:" + String(gps.location.lng(), 6) + "\t";
    } else {
      dataStringSend += "LAT:~\tLON:~\t";
    }

    if (gps.time.isValid()) {
      int hora = gps.time.hour();
      int minuto = gps.time.minute();

      // Corrige para UTC-3
      hora = (hora - 3 + 24) % 24;

      dataStringSend += "HR:" + String(hora) + "\t";
      dataStringSend += "M:" + String(minuto) + "\t";
    } else {
      dataStringSend += "HR:~\tM:~\t";
    }

    if (gps.hdop.isValid()) {
      dataStringSend += "PR:" + String(gps.hdop.hdop(), 1) + "\t";
    } else {
      dataStringSend += "PR:~\t";
    }
#else
    dataStringSend += "LAT:~\tLON:~\tHR:~\tM:~\tPR:~\t";
#endif

#if (USE_BMP280)
    dataStringSend += "H:" + String(altitude, 2) + "\t";
#else
    dataStringSend += "H:0\t";
#endif

#if (SD_CARD)
    dataStringSend += "SD:" + String(sd_bool) + "\t";
#else
    dataStringSend += "SD:0\t";
#endif

#if (APOGEU)
    dataStringSend += "AH:";
    dataStringSend += String(apogeu.getMaxAltitude(), 2) + "\t";
    dataStringSend += "AT:";
    dataStringSend += String(apogeu.getApogeuTime() / 1000.0, 2) + "\t";
#else
    dataStringSend += "AH:0\tAT:0\t";
#endif

#if (PARAQUEDAS)
#if (PQD_DN)
    dataStringSend += "DN:";
    dataStringSend += String(pqd_dn.getPqdHeight(), 2) + "\t";
    // dataStringSend += String(pqd_dn.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (PQD_DB)
    dataStringSend += "DB:";
    dataStringSend += String(pqd_db.getPqdHeight(), 2) + "\t";
    // dataStringSend += String(pqd_db.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (PQD_MN)
    dataStringSend += "MN:";
    dataStringSend += String(pqd_mn.getPqdHeight(), 2) + "\t";
    // dataStringSend += String(pqd_mn.getPqdTime() / 1000.0, 2) + "\t";
#endif
#if (PQD_MB)
    dataStringSend += "MB:";
    dataStringSend += String(pqd_mb.getPqdHeight(), 2) + "\t";
    // dataStringSend += String(pqd_mb.getPqdTime() / 1000.0, 2) + "\t";
#endif


#else
    dataStringSend += "DN:0\t";
    dataStringSend += "DB:0\t";
    dataStringSend += "MN:0\t";
    dataStringSend += "MB:0\t";
#endif

#if (USE_BMP280)
    dataStringSend += "TP:" + String(temperatura, 2) + "\t";
#else
    dataStringSend += "TP:0\t";
#endif

#if (ENABLE_3D)
    dataStringSend += "R:" +String(roll, 2) + "\t";
    dataStringSend += "P:" +String(pitch, 2) + "\t";
    dataStringSend += "Y:" +String(yaw, 2) + "\t";
#else
    dataStringSend += "R:0\t";
    dataStringSend += "P:0\t";
    dataStringSend += "Y:0\t";
#endif


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
#if (PQD_DN)
  pqd_dn.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //pqd_dn.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

  //Paraquedas 2
#if (PQD_DB)
  pqd_db.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //pqd_db.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

//Paraquedas 3
#if (PQD_MN)
  pqd_mn.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //pqd_mn.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

//Paraquedas 4
#if (PQD_MB)
  pqd_mb.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo());
  //pqd_mb.ativarParaquedas(alturaReal, currentTime, apogeu.getEstaDescendo(), bmp.getEstaDescendoTimer());
#endif

#endif
}


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

#if SERIAL_PRINT
  delay(500); 
#endif

#if (LORA)
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
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
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
  GPS.begin(GPS_BAUD, SERIAL_8N1, GPS_TX, GPS_RX);
#else
  GPS.begin(GPS_BAUD);
#endif
#endif  // GPS


#if ((GYRO) || (MAG) || (ACCEL) || (BMP180) || (USE_BMP280))
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
  Wire.begin(SDA_PIN, SCL_PIN);
#else
  Wire.begin();
#endif
#endif

delay(100); // Delay para envio de dados

#if CAM
  pinMode(CAM_PIN, OUTPUT);
#endif  // CAM

#if BUZZER
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, !BUZZ_ON); 
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

#elif defined(ARDUINO_ARCH_ESP32)
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);  // inicia os pinos
  if (!SD.begin(SD_CS, SPI)) {

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
  unsigned long t_atual = millis();
  String baseName = NOME_ARQUIVO;
  while (true) {
    if (millis() - t_atual > 10000) {
      strcpy(nomeSD, " err");
      break;
    }

    // calcula quantos zeros precisa
    int numZeros = 8 - baseName.length() - String(iSD).length();
    String zeros = "";
    for (int i = 0; i < numZeros-1; i++) zeros += "0";

    // monta o nome do arquivo
    String nome = "/" + baseName + zeros + String(iSD) + ".txt";
    nome.toCharArray(nomeSD, sizeof(nomeSD));
    // Serial.println(String(nomeSD) + " testando.");

    if (SD.exists(nomeSD)) {
      // já existe → tenta próximo
      
    } else {
      File dataFile = SD.open(nomeSD, FILE_APPEND);
      if (dataFile) {
        Serial.println(String(nomeSD) + " criado.");
      } else {
        Serial.println(String(nomeSD) + " err");
      }
      break;
    }
    iSD++;
  }
#endif

#if (PQD_DN)
  pqd_dn.begin();
#endif

#if (PQD_DB)
  pqd_db.begin();
#endif

#if (PQD_MN)
  pqd_mn.begin();
#endif

#if (PQD_MB)
  pqd_mb.begin();
#endif

#if (REINICIO)
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
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

#if (REINICIO)
  bool valorOk = true;
  if ((pressaoInicial >= 100000 || pressaoInicial <= 85000)) {
    valorOk = false;
  }
  if (!valorOk && vooAtivo) {
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
    pressaoInicial = prefs.getFloat("pressaoIni", 0);  // testar
    altitudeInicial = getHeight(pressaoInicial);
    sim.restart(pressaoInicial, altitudeInicial);
#endif  // ARCH ESP32
    Serial.println(F("RESTART PROTECTION"));
  } else {
    vooAtivo = false;
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
    prefs.putFloat("pressaoIni", pressaoInicial);
    prefs.putBool("vooAtivo", vooAtivo);
#endif  // ARCH ESP32
    Serial.println(F("NO RESTART PROTECTION"));
  }
#endif  // REINICIO

#if SD_CARD
  dataFile = SD.open(nomeSD, FILE_APPEND);
  if (dataFile) {
    dataFile.print(F("Baro starts: "));
    dataFile.println(pressaoInicial);
    dataFile.flush();
    dataFile.close();
  }
#endif
#endif

#if (ENABLE_3D)
  filter.begin(FREQ_3D);  // taxa de amostragem em Hz
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
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
    pressaoInicial = prefs.getFloat("pressaoIni", 0);  // testar
    altitudeInicial = getHeight(pressaoInicial);
    sim.restart(pressaoInicial, altitudeInicial);
#endif  // ARCH ESP32
    Serial.println(F("RESTART PROTECTION"));
  } else {
    vooAtivo = false;
#if defined(PLACA_ESSENTIALS_V1) || defined(PLACA_MAIN_PTH) || defined(PLACA_MAIN_SMD) || defined(PLACA_MAIN_IC)
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

#if (USE_DHT)
  header += "tdht_C\t";
  header += "umidade_%\t";
#endif

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

#if (GPS_ON)
  header += "lat.GPS.deg\t";
  header += "lon.GPS.deg\t";
  header += "alt.GPS.m\t";
  header += "vel.GPS.kmph\t";
  header += "hora.GPS\t";
  header += "min.GPS\t";
  header += "precisao.GPS\t";
#endif

#if (SD_CARD)
  dataFile = SD.open(nomeSD, FILE_APPEND);
  if (dataFile) {
    dataFile.println(header);
    dataFile.flush();
    dataFile.close();
  } else {
    Serial.print("Erro ao abrir ");
    Serial.println(nomeSD);
  }
#endif

#if (APOGEU)
  header += "descendo\t";
  header += "altApogee_m\t";
  header += "tApogee_s\t";
  header += "altMax_m\t";
#endif


#if (PARAQUEDAS && PQD_DN)
  header += "p1_data\t";
#if (SAUDE)
  header += "p1_info\t";
#endif
#endif

#if (PARAQUEDAS && PQD_DB)
  header += "p2_data\t";
#if (SAUDE)
  header += "p2_info\t";
#endif
#endif

#if (PARAQUEDAS && PQD_MN)
  header += "p3_data\t";
#if (SAUDE)
  header += "p3_info\t";
#endif
#endif

#if (PARAQUEDAS && PQD_MB)
  header += "p4_data\t";
#if (SAUDE)
  header += "p4_info\t";
#endif
#endif

  // header += "\n";

  Serial.println(header);


#if (LORA)
  String sendHeader = "linha\t";
  sendHeader += "tempo.s\t";

#if (GPS_ON)
  sendHeader += "lat.GPS.deg\tlon.GPS.deg\t";
  sendHeader += "hora.GPS\tmin.GPS\tprecisao.GPS\t";
#endif  // GPS_ON

#if (USE_BMP280)
  sendHeader += "baro.h.m\t";
#endif  // USE_BMP280

#if (SD_CARD)
  sendHeader += "sd.bool\t";
#endif

#if (APOGEU)
  sendHeader += "apogeu.h.m\t";
  sendHeader += "apogeu.t.s\t";
#endif  // APOGEU

#if (PQD_MN)
  sendHeader += "pqd_mn.h.m\tpqd_mn.t.s\t";
  // sendHeader += "pqd_mn.t.s\t";
#endif

#if (PQD_DN)
  sendHeader += "pqd_dn.h.m\t";
  // sendHeader += "pqd_dn.t.s\t";
#endif

#if (PQD_MB)
  sendHeader += "pqd_mb.h.m\t";
  // sendHeader += "pqd_mb.t.s\t";
#endif

#if (PQD_DB)
  sendHeader += "pqd_db.h.m\t";
  // sendHeader += "pqd_db.t.s\t";
#endif

#if (USE_BMP280)
  sendHeader += "baro.t.c";
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