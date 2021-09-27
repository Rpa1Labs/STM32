

//Header files
#include "DHT22.h"


//Change pin mode
static void ChangePinMode(uint8_t mode)
{
    HAL_GPIO_DeInit(GPIO_DHT22, PIN_DHT22);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = PIN_DHT22;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    if(mode==1) GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    else GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(GPIO_DHT22, &GPIO_InitStruct);
}

//DHT Begin function
static void DHT22_StartAcquisition(void)
{
    //Number of iterations for 30uSec
    uint32_t whileIterations = 30 * ((SystemCoreClock / 1000000)/3);

    //Change data pin mode to OUTPUT
    ChangePinMode(1);

    //Put pin LOW
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 0);

    //2mSec delay
    HAL_Delay(2);

    //Bring pin HIGH
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 1);

    //30 uSec delay
    while (whileIterations--);

    //Set pin as input
    ChangePinMode(0);
}

//Read 5 bytes
static void DHT22_ReadRaw(uint8_t *data)
{
    uint32_t rawBits = 0UL;
    uint8_t checksumBits = 0;
    uint32_t delayWhile = (SystemCoreClock / 800000);

    while (!HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22));
    while (HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22));

    for (int8_t i = 31; i >= 0; i--){
        uint32_t iterationCount = 0;
        while (READ_PIN==0);

        while (READ_PIN>0){
            iterationCount++;
        }
        if (iterationCount>delayWhile)
        {
            rawBits |= (1UL << i);
        }
    }

    for (int8_t i = 7; i >= 0; i--)
    {
        uint32_t iterationCount = 0;
        while (READ_PIN==0);

        while (READ_PIN>0){
            iterationCount++;
        }
        if (iterationCount>delayWhile)
        {
            checksumBits |= (1UL << i);
        }
    }

    //Copy raw data to array of bytes

    //Humidity
    data[0] = rawBits >> 24 ;
    data[1] = rawBits >> 16 ;
    //Temperature
    data[2] = rawBits >> 8  ;
    data[3] = rawBits >> 0  ;
    //Checksum
    data[4] = checksumBits  ;
}

//Get Temperature and Humidity data
uint8_t DHT22_GetTemp_Humidity(uint16_t *Temp, uint16_t *Humidity)
{
    __GPIOA_CLK_ENABLE();
    uint8_t dataArray[5], checkSum;
    
    //Implement Start data Aqcuisition routine
    DHT22_StartAcquisition();
    
    //Aqcuire raw data
    DHT22_ReadRaw(dataArray);

    //Calculate checksum
    checkSum = 0;
    for (uint8_t k = 0; k < 4; k++)
    {
        checkSum += dataArray[k];
    }
    
    //If checksum is valid
    if (checkSum == dataArray[4])
    {
        *Temp = (dataArray[2] << 8) | dataArray[3];
        *Humidity = (dataArray[0] << 8) | dataArray[1];
        return 1;
    }
    return 0;
}
