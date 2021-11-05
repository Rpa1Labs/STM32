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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = 16;


  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    //Error
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

// FIN FONCTIONS ESSENTIELLES POUR LE GPIO */




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


void UARTTest(){
  /*Serial serie;
  Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  uint8_t tempMsg[] = " Temperature: ";
  Send(&serie, &tempMsg[0], sizeof(tempMsg));*/
}

void readTemp_Send(){
  InitRcSwitch();
  enableTransmit();
  
  HAL_Delay(2000);

  uint16_t Temp = 0.0;
  uint16_t Hyd = 0.0;
  while (1)
  {
    DHT22_GetTemp_Humidity(&Temp, &Hyd);

    send((uint32_t) Temp,24);

    HAL_Delay(4000);
  }
}



int main()
{



  SystemClock_Config();

  HAL_Init();

  //MX_GPIO_Init();

  //UARTTest();

  //LED_Loop();

  //DHT22ReadAndPrint();

  //SpiTest();

  //RCSwitchTest();
  
  readTemp_Send();

  return 0;
}
