#include "main.h"

#include "DHT22.h"
#include "RCSwitch.h"
#include "ControlLED.h"
#include "rtc.h"

/** DEBUT DES FONCTIONS ESSENTIELLES POUR LA STM32 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

void SystemClock_Config(void)
{
  
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __PWR_CLK_ENABLE();

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    LED_Loop();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    LED_Loop();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    LED_Loop();
  }
}

/** FIN DES FONCTIONS ESSENTIELLES POUR LA STM32 */

// FONCTION EXECUTEE LORS DE L'INTERRUPT DE L'ALARME A
void RTC_INTERRUPT()
{
  SystemClock_Config();
  
  HAL_ResumeTick();

  // Execute la fonction de lecture de température et d'envoi
  readTemp_Send();

  // Re-initialise la rtc et l'alarme pour réexécuter cette fonction dans 1 minute
  Set_Time();
  Set_Alarm();
}


void readTemp_Send()
{
  HAL_Init();
  InitRcSwitch();
  enableTransmit();

  uint16_t Temp = 0;
  uint16_t Hyd = 0;
  uint8_t status = 0;

  while(!status){
    status = DHT22_GetTemp_Humidity(&Temp, &Hyd);
  }

  LED_Init();
  HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

  uint32_t sendData = Temp;

  // Ajout d'un header pour éviter les conflits
  sendData |= (HEADER << 16);

  send(sendData,24);

  
}

void sleep()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Met tous les pins en mode analog (le moins consommateur)
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


  // Met au maximum tous les diviseurs d'horloge
  // Pour avoir le fréquence la plus petite
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV512;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    LED_Loop();
  }

  // Désactive toutes les horloges des gpios
  __HAL_RCC_PWR_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __PWR_CLK_DISABLE();
  HAL_SuspendTick();

  // Entre en mode STANDBY
  // HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  HAL_PWR_EnterSTANDBYMode();
}

int main()
{

  SystemClock_Config();

  HAL_Init();
  LED_Init();

  readTemp_Send();

  HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
  HAL_Delay(2000);
  HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

  RTC_Init();

  while (1)
  {
    sleep();
  }

  return 0;
}
