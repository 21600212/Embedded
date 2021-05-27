#include "mbed.h"

SPI spi (ARDUINO_UNO_D11, ARDUINO_UNO_D12, ARDUINO_UNO_D13);
DigitalOut cs(PA_4);

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
AnalogIn ain0(ARDUINO_UNO_A0);
