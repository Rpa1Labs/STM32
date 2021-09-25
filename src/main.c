#include "main.h"

#include "Serial.h"

/** DEBUT FONCTIONS ESSENTIELLES POUR LE GPIO */
void SysTick_Handler(void)
{
    HAL_IncTick();
}
/** FIN FONCTIONS ESSENTIELLES POUR LE GPIO */

/** DEBUT CONTRÔLE LED */

void LED_Init() {
  LED_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void LED_Loop(){
  HAL_Init();
  LED_Init();

  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    HAL_Delay(1000);
  }
}
/** FIN CONTRÔLE LED */

Serial serie;

int main(){
  //LED_Loop();
  Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  HAL_Delay(1000);
  
  
  
  //uint8_t Test[] = "Bite !!!\r\n"; //Data to send

  uint8_t* Test = malloc(4*sizeof(uint8_t));

  for(int i=0;i<4;i++){
    Test[i] = 'a';
  }

  uint8_t Okmsg[] = "OK\r\n";

  Send(&serie,&Okmsg[0],sizeof(Okmsg));

  Receive(&serie,Test,4,2000);

  Send(&serie,&Okmsg[0],sizeof(Okmsg));

  while(1){
    Send(&serie,&Test[0],4);
    HAL_Delay(1000);
  }
  return 0;
}
