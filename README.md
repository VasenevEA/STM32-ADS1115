# STM32-ADS1115
STM32 library for read result ADC from ADS1115 with i2c protocol

usig MCU - Stm32F407VGT6

Methods:

void InitADS1115(); // You realisation I2C by default it's I2C1 pins PB6 PB7

uint_fast16_t getData(uint8_t number);
void selectInput(uint8_t number);
