#include "main.h"

//#include "Serial.h"

//#include "DHT22.h"

//#include "SPI.h"

//#include "RCSwitch.h"

//#include "ControlLED.h"

// TODO FAIRE LA MISE EN VEILLE
// TODO EXECUTER LA FONCTION readTemp_Send() PENDANT l'INTERRUPTION

#include "rtc.h"

/** DEBUT DES FONCTIONS ESSENTIELLES POUR LE STM32 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Clear Wake Up Flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

void SystemClock_Config(void){
    
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    //Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    //Error_Handler();
  }

  __HAL_RCC_RTC_ENABLE();
    /* RTC interrupt Init */
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


/** FIN DES FONCTIONS ESSENTIELLES POUR LE STM32 */


void DHT22ReadAndPrint(){
  /*Serial serie;
  Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  HAL_Delay(1000);

  uint8_t tempMsg[] = " Temperature: ";
  uint8_t hydMsg[] = " Humidite: ";

  uint16_t Temp = 0.0;
  uint16_t Hyd = 0.0;

  while (1)
  {
    DHT22_GetTemp_Humidity(&Temp, &Hyd);

    Send(&serie, &tempMsg[0], sizeof(tempMsg));
    PrintInt(&serie, Temp);
    Send(&serie, &hydMsg[0], sizeof(hydMsg));
    PrintInt(&serie, Hyd);
    
    NextLine(&serie);

    HAL_Delay(4000);
  }*/
}



void SpiTest(){
  /*SpiInit();

  char test[] = "test";

  while(1){
    SpiSend(&test[0],sizeof(test));
    HAL_Delay(2000);
  }*/
}


/**
void RCSwitchTest(){
  InitRcSwitch();
  enableTransmit();
  HAL_Delay(2000);
  unsigned long var = 0;
  while(1){
    var = (var+1)%65000;
    send(var, 24);
    HAL_Delay(2000);
  }
}
*/

void UARTTest(){
  /*Serial serie;
  Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  uint8_t tempMsg[] = " Temperature: ";
  Send(&serie, &tempMsg[0], sizeof(tempMsg));*/
}
/*
void readTemp_Send(){
  InitRcSwitch();
  enableTransmit();
  
  HAL_Delay(2000);

  uint16_t Temp = 0.0;
  uint16_t Hyd = 0.0;
  while (1)
  {
    //set_time(0);
    DHT22_GetTemp_Humidity(&Temp, &Hyd);

    send((uint32_t) Temp,24);

    //Enter_Standby_Mode();
    sleepdelay(4);
    //HAL_Delay(4000);
  }
}
*/




int main()
{



  SystemClock_Config();

  LED_Init();

  HAL_Init();
  //LED_Loop();

  MX_RTC_Init();
  

  HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

while(1);
  

  //UARTTest();

  //LED_Loop();

  //DHT22ReadAndPrint();

  //SpiTest();

  //RCSwitchTest();
  
  //readTemp_Send();

  return 0;
}
