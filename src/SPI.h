#include"stm32f3xx_hal.h"

#define Spi SPI_HandleTypeDef

#define PIN_CLK     GPIO_PIN_5
#define PIN_MISO    GPIO_PIN_6
#define PIN_MOSI    GPIO_PIN_7

void SpiInit();
void SpiSend(uint8_t* data,int size);
void SpiReceive(uint8_t* data, int size, int timeOut);