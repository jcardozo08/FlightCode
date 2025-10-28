#pragma once
#include <Arduino.h>

class Simulador  
{
    float pressao;
    float altura;
    float alturaInicial;
    float pressaoInicial;
    const int baud;
    const int ledPin = -1;


public:
    Simulador(int baud_rate, int led_pin = -1);
    String getDataString();
    String getHeaderString();
    float getP0();
    float getHeight(float pressure);
    float loop();
    void restart(float p0, float h0);

private:
    void begin(); // Inicia ao conectar usb do micro
    float getInitialHeight(float pressure);
};