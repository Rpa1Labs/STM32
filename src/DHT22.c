

//Header files
#include "DHT22.h"


//Change le pin mode
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


static void DHT22_StartAcquisition(void)
{
    //Nombre d'iterations pour 30uSec
    uint32_t whileIterations = 30 * ((SystemCoreClock / 1000000)/3);

    //Met le pin en sortie
    ChangePinMode(1);

    //Force la mise à 0 du pin
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 0);

    //délai de 2 millisecondes
    HAL_Delay(2);

    //Met le pin à Vcc
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 1);

    //Délai de 30 microsecondes
    while (whileIterations--);

    //Met le pin en entrée
    ChangePinMode(0);
}

//Lecture des 5 octets
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

    //Copie les données brutes dans le data

    //Humidity
    data[0] = rawBits >> 24 ;
    data[1] = rawBits >> 16 ;
    //Temperature
    data[2] = rawBits >> 8  ;
    data[3] = rawBits >> 0  ;
    //Checksum
    data[4] = checksumBits  ;
}


uint8_t DHT22_GetTemp_Humidity(uint16_t *Temp, uint16_t *Humidity)
{
    __GPIOA_CLK_ENABLE();
    uint8_t dataArray[5], checkSum;
    
    //Envoie le signal au capteur pour demander l'envoi des données
    DHT22_StartAcquisition();
    
    //Récupère les données brutes
    DHT22_ReadRaw(dataArray);

    //Calcule la somme de contrôle
    checkSum = 0;
    for (uint8_t k = 0; k < 4; k++)
    {
        checkSum += dataArray[k];
    }
    
    //Si la somme de contrôle est valide
    if (checkSum == dataArray[4])
    {
        *Temp = (dataArray[2] << 8) | dataArray[3];
        *Humidity = (dataArray[0] << 8) | dataArray[1];
        return 1;
    }
    return 0;
}
