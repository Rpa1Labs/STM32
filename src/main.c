#include "main.h"

//#include "Serial.h"

//#include "DHT22.h"

//#include "SPI.h"

#include "RCSwitch.h"

#include "ControlLED.h"

/** DEBUT FONCTIONS ESSENTIELLES POUR LE GPIO */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void SystemClock_Config(void){

  RCC->CIR = 0x009F0000;

  // Turn HSE ON
  RCC->CR |= RCC_CR_HSEON;

  // Wait Until HSE Is Ready
  while (!(RCC->CR & RCC_CR_HSERDY));

  //set HSE as system clock
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_HSE;

  //AHB prescaler
  RCC->CFGR &= ~(RCC_CFGR_HPRE);   //remove old prescaler
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //set AHB prescaler = 1.

  //set APB1 prescaler
  RCC->CFGR &= ~(RCC_CFGR_PPRE1);
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

  //set APB2 prescaler
  RCC->CFGR &= ~(RCC_CFGR_PPRE2);
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

  //set flash wait states to 2 wait states
  FLASH->ACR &= ~(FLASH_ACR_LATENCY);
  FLASH->ACR |= FLASH_ACR_LATENCY_2;

  //Set HSE as PLL Source. bit set -> HSE, bit unser -> HSI
  RCC->CFGR |= RCC_CFGR_PLLSRC;

  // Set HSE Prescaler On PLL Entry
  RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
  // RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE; //no HSE prescaler before PLL entry

  // Turn On PLL Clock
  RCC->CR |= RCC_CR_PLLON;

  // Wait Until PLL Is Ready
  while (!(RCC->CR & RCC_CR_PLLRDY));

  // Set System To PLL CLock
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL;

  // Clear All Interrupts
  RCC->CIR = 0x009F0000;
}

// FIN FONCTIONS ESSENTIELLES POUR LE GPIO */

//Serial serie;

int main()
{

  //LED_Loop();
  SystemClock_Config();

  LED_Loop();
  
  /*Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  HAL_Delay(1000);

  uint8_t tempMsg[] = " Temperature: ";
  uint8_t hydMsg[] = " Humidite: ";*/

  /*Receive(&serie,Test,4,2000);*/

  /*uint16_t Temp = 0.0;
  uint16_t Hyd = 0.0;*/

  /*while (1)
  {
    DHT22_GetTemp_Humidity(&Temp, &Hyd);

    Send(&serie, &tempMsg[0], sizeof(tempMsg));
    PrintInt(&serie, Temp);
    Send(&serie, &hydMsg[0], sizeof(hydMsg));
    PrintInt(&serie, Hyd);
    
    NextLine(&serie);

    HAL_Delay(4000);
  }*/

  /*SpiInit();

  char test[] = "test";

  while(1){
    SpiSend(&test[0],sizeof(test));
    HAL_Delay(2000);
  }*/
  //RCSwitch mySwitch = RCSwitch();

  /*mySwitch.enableTransmit(0);

  while(1){
    mySwitch.send(12345678, 24);
    HAL_Delay(2000);
  }*/

  return 0;
}
