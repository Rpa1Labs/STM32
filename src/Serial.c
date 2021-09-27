#include "Serial.h"

void Init(Serial *self,int baudRate, uint16_t PIN_Tx, uint16_t PIN_Rx){

  HAL_Init();

  GPIO_InitTypeDef  GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();

  // INITIALISATION DES PINs TX ET RX
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
  GPIO_InitStruct.Pin       = PIN_Tx | PIN_Rx;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // Initialisation de l'UART

  __USART2_CLK_ENABLE();

  self->Instance          = USART2;

  self->Init.BaudRate     = baudRate;
  self->Init.WordLength   = UART_WORDLENGTH_8B;
  self->Init.StopBits     = UART_STOPBITS_1;
  self->Init.Parity       = UART_PARITY_NONE;
  self->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  self->Init.Mode         = UART_MODE_TX_RX;
  self->Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(self) != HAL_OK) {
    while(1){}
  }

  // SETUP DES INTERRUPTIONS POUR LE RECEPTION DES DONNEES
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

}

void Send(Serial* self, uint8_t* data,int sizeInBytes){
    HAL_UART_Transmit(self, data, sizeInBytes,999999);
}


void Receive(Serial* self, uint8_t* data, int sizeInBytes, int timeOut){
    HAL_UART_Receive(self, data, sizeInBytes,timeOut);
}

void End() {
  __USART2_CLK_DISABLE();
  __GPIOA_CLK_DISABLE();
}

void PrintInt(Serial* self,uint32_t nbSend){
  int nbDigit = 0;
  uint32_t number = nbSend;
  while(number != 0) {
      number = number / 10;
      nbDigit++;
  }
  uint32_t mask = 0;
  for(int i=0; i<=nbDigit;i++){
      uint16_t digit = nbSend /pow(10 ,nbDigit-1-i);
      char result = (char)digit-mask*10 + '0';
      Send(self,&result,sizeof(char));
      mask = digit;

  }
}

void NextLine(Serial* self){
  char nextLine[] = "\r\n";
  Send(self,&nextLine,sizeof(nextLine));
}
