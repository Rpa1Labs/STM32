/*
Library:					DHT22 - AM2302 Temperature and Humidity Sensor
Written by:				Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Date Written:			21/11/2018
Last modified:		-/-
Description:			This is an STM32 device driver library for the DHT22 Temperature and Humidity Sensor, using STM HAL libraries
References:				This library was written based on the DHT22 datasheet
										- https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf
										
* Copyright (C) 2018 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.
*/

//Header files
#include "DHT22.h"

//#include "Serial.h"

//#define CLOCKFACTOR     (SystemCoreClock / 1000000) / 3

//Bit fields manipulations
/*#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))*/

//1. One wire data line
/*static GPIO_TypeDef *oneWire_PORT;
static uint16_t oneWire_PIN;
static uint8_t oneWirePin_Idx;*/

//*** Functions prototypes ***//
//OneWire Initialise
/*void DHT22_Init(GPIO_TypeDef *DataPort, uint16_t DataPin)
{
    oneWire_PORT = DataPort;
    oneWire_PIN = DataPin;
    for (uint8_t i = 0; i < 16; i++)
    {
        if (DataPin & (1 << i))
        {
            oneWirePin_Idx = i;
            break;
        }
    }
}*/

/** DEBUT DEBUG SERIE */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_BAUDRATE                  115200
//Serial serie;
/** FIN DEBUG SERIE */


void InitDebug(){
    /*Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);

    uint8_t okdebug[] = "OK Debug";
    Send(&serie,&okdebug[0],sizeof(okdebug));*/
    __GPIOA_CLK_ENABLE();
}


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


//One Wire pin HIGH/LOW Write
/*static void ONE_WIRE_Pin_Write(uint8_t state)
{
    if (state)
        HAL_GPIO_WritePin(oneWire_PORT, oneWire_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(oneWire_PORT, oneWire_PIN, GPIO_PIN_RESET);
}*/


static uint8_t ONE_WIRE_Pin_Read(void)
{
    return HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22);
}

//Microsecond delay
static void DelayMicroSeconds(uint32_t uSec)
{
    uint32_t uSecVar = uSec;
    uSecVar = uSecVar * ((SystemCoreClock / 1000000)/3 );
    while (uSecVar--);
}

//DHT Begin function
static void DHT22_StartAcquisition(void)
{
    uint32_t uSecVarLong = 1500 * ((SystemCoreClock / 1000000)/3);
    uint32_t uSecVarShort = 30 * ((SystemCoreClock / 1000000)/3);
    //Change data pin mode to OUTPUT
    ChangePinMode(1);
    //Put pin LOW
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 0);
    //500uSec delay
    //DelayMicroSeconds(10000);
    HAL_Delay(10);
    while (uSecVarLong--);
    //Bring pin HIGH
    HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, 1);
    //30 uSec delay
    //DelayMicroSeconds(30);
    while (uSecVarShort--);

    //HAL_GPIO_WritePin(GPIO_DHT22, PIN_DHT22, GPIO_PIN_RESET);
    //Set pin as input
    ChangePinMode(0);
}
//Read 5 bytes
static void DHT22_ReadRaw(uint8_t *data)
{
    uint32_t rawBits = 0UL;
    uint8_t checksumBits = 0;
    uint32_t delayWhile = (SystemCoreClock / 800000);

    //DelayMicroSeconds(40);

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
    data[0] = (rawBits >> 24);
    data[1] = (rawBits >> 16);
    data[2] = (rawBits >> 8);
    data[3] = (rawBits >> 0);
    data[4] = (checksumBits);
}

//Get Temperature and Humidity data
uint8_t DHT22_GetTemp_Humidity(float *Temp, float *Humidity)
{
    uint8_t dataArray[5], myChecksum;
    uint16_t Temp16, Humid16;
    //Implement Start data Aqcuisition routine
    DHT22_StartAcquisition();
    //Aqcuire raw data
    DHT22_ReadRaw(dataArray);
    //calculate checksum
    myChecksum = 0;
    for (uint8_t k = 0; k < 4; k++)
    {
        myChecksum += dataArray[k];
    }
    //Send(&serie,&dataArray[4],sizeof(uint8_t));
    if (myChecksum == dataArray[4])
    {
        Temp16 = (dataArray[2] << 8) | dataArray[3];
        Humid16 = (dataArray[0] << 8) | dataArray[1];

        *Temp = Temp16 / 10.0f;
        *Humidity = Humid16 / 10.0f;
        return 1;
    }
    return 0;
}
