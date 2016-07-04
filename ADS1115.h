#include "stm32f4xx_i2c.h"
#include "ADS1115_LowLevel.h"


/*I2C1
 * PB6 - SCL
 * PB7 - SDA
 * Speed- 200 000 Hz
 */

void InitADS1115();

uint_fast16_t getData(uint8_t number);
void selectInput(uint8_t number);
