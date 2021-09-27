#include "main.h"

#include "Serial.h"

#include "DHT22.h"

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
  //__GPIOB_CLK_ENABLE();
  //LED_Loop();
  SystemClock_Config();
  Init(&serie, USARTx_BAUDRATE, USARTx_TX_PIN, USARTx_RX_PIN);
  HAL_Delay(1000);

  /*uint8_t* Test = malloc(4*sizeof(uint8_t));

  for(int i=0;i<4;i++){
    Test[i] = 'a';
  }*/

  uint8_t Okmsg[] = "OK\r\n";
  
  Send(&serie,&Okmsg[0],sizeof(Okmsg));

  /*Receive(&serie,Test,4,2000);*/


  float Temp = 0.0;
  float Hyd = 0.0;

  uint8_t final = 0;

  InitDebug();


  //Send(&serie,&Okmsg[0],sizeof(Okmsg));

  while(1){
    DHT22_GetTemp_Humidity(&Temp, &Hyd);
    Send(&serie,&Okmsg[0],sizeof(Okmsg));
    final = (int)Hyd + '0';
    Send(&serie,&final,1);
    HAL_Delay(4000);
  }
  return 0;
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  //__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_LOWPOWERREGULATOR_ON);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = 16;
  /*RCC_OscInitStruct.PLL.PLLNUL = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;*/
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
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
    //_Error_Handler(__FILE__, __LINE__);
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
