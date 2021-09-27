//Header files
#include "stm32f3xx_hal.h"


#define     GPIO_DHT22        GPIOA
#define     PIN_DHT22         GPIO_PIN_9


#define     READ_PIN          HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22)


//Change pin mode true -> output, false -> input
static void ChangePinMode(uint8_t mode);
//Send signal to DHT22
static void DHT22_StartAcquisition(void);
//Read 5 bytes (40bits)
static void DHT22_ReadRaw(uint8_t *data);
//Get Temperature and Humidity data
uint8_t DHT22_GetTemp_Humidity(uint16_t *Temp, uint16_t *Humidity);