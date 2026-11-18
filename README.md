# FlightCode

Firmware de voo **modular** para computadores de bordo de foguetes. Um único código-fonte compila para várias placas diferentes: a placa e os sensores são escolhidos em tempo de compilação, então a mesma lógica de voo validada roda em todas as minhas placas.

## Placas suportadas

Definidas em `src/lib/pinout.h`:
- `PLACA_ESSENTIALS_V1`: [Essentials](https://github.com/jcardozo08/Essentials)
- `PLACA_MAIN_PTH`: [Essentials-V2](https://github.com/jcardozo08/Essentials-V2)
- `PLACA_MAIN_SMD`: [PlacaMainSMD_Simplificada](https://github.com/jcardozo08/PlacaMainSMD_Simplificada)
- `PLACA_MAIN_IC`: [PlacaMainSMD_v1](https://github.com/jcardozo08/PlacaMainSMD_v1)
- `PLACA_MAIN_OCTA_PTH`

## Funcionalidades

- Leitura de barômetro (BMP280/BMP085) e IMU (MPU9250 + AK8963 / GY-91)
- Altitude calculada em relação à pressão média no solo, com filtro de Kalman
- Detecção de apogeu e acionamento dos canais de recuperação
- Recuperação do estado de voo após reinício inesperado (Preferences no ESP32)
- Modo simulador para testar a lógica de voo em bancada

## Configuração

- `src/lib/config.h`: liga/desliga sensores e funções (`GYRO`, `ACCEL`, `MAG`, `USE_BMP280`, `REINICIO`, `SIMULADOR`...)
- `src/lib/pinout.h`: pinagem de cada placa

**Stack:** C++ · Arduino · ESP32

> Evoluiu para o [FlightComputerRTOS](https://github.com/jcardozo08/FlightComputerRTOS), que reorganiza essa lógica em tasks FreeRTOS.
