#include "SPI.h"

Spi hspi1;

/* SPI1 init function */ 
void SpiInit() 
{ 

     __GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin       = PIN_CLK | PIN_MISO | PIN_MOSI;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    __SPI1_CLK_ENABLE();
    hspi1.Instance = SPI1;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
    hspi1.Init.Mode = SPI_MODE_MASTER;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        asm("bkpt 255");
    }
 
}


void SpiSend(uint8_t* data,int size){ 
    HAL_SPI_Transmit(&hspi1, data, size, 9999999); 
}

void SpiReceive(uint8_t* data, int size, int timeOut){
    HAL_SPI_Receive (&hspi1, data, size, timeOut); 
}