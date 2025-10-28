#include "sim.h"

#if !defined(ARDUINO_ARCH_ESP32)
#include <avr/wdt.h>
#endif


Simulador::Simulador(int baud_rate, int led_pin) 
  : baud(baud_rate), ledPin(led_pin) {}

void Simulador::begin() {
    Serial.begin(baud);
    delay(2500);
    if (ledPin != -1) {
        for (int i = 0; i < 2; i++) {
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);
            delay(100);
        }
    }
    
    Serial.println(F("READY"));
}


float Simulador::getP0() {
    begin();

    float soma = 0;
    uint8_t contagem = 0;
    for (int i = 0; i < 10; i++) {
        while (!Serial.available()) {
        delay(1);
        }
        float valor = Serial.parseFloat();
        soma += valor;
        contagem++;
    }
    pressaoInicial  = soma / contagem;
    alturaInicial = getInitialHeight(pressaoInicial);
    Serial.print(F("Simulador Interno - Pressao inicial / Altura inicial: "));
    Serial.print(pressaoInicial);
    Serial.print(" Pa \t");
    Serial.print(alturaInicial);
    Serial.println(" m");
    return pressaoInicial;
}

void Simulador::restart(float p0, float h0) {
    pressaoInicial = p0;
    alturaInicial = getInitialHeight(pressaoInicial);
    Serial.print("Debug: ");
    Serial.print(pressaoInicial);
    Serial.print(" Pa ");
    Serial.print(alturaInicial);
    Serial.println(" m");
}

float Simulador::getHeight(float pressure) {
    altura = 44330.0 * (1 - pow(float(pressure) / 101325, 1.0 / 5.25588)) - alturaInicial;
    return altura;
}

float Simulador::getInitialHeight(float pressure) {
    float alt = 44330.0 * (1 - pow(float(pressure) / 101325, 1.0 / 5.25588));
    return alt;
}

float Simulador::loop() {
    if (Serial.available()) {
        pressao = Serial.parseFloat();
    }
    return pressao;
}

String Simulador::getDataString() {
  String data;
  data += String(pressao, 2) + '\t';
  data += String(altura, 2) + '\t';
  return data;
}

String Simulador::getHeaderString() {
  String header;
  header += "pressao_hPa\t";
  header += "alt_m\t";
  return header;
}
