#include "rtc.h"

//#include "RCSwitch.h"

#include "ControlLED.h"


// INITIALISATION DU RTC
void MX_RTC_Init(void)
{
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    //Error_Handler();
  }

  //Setup du temps et de l'alarme  
  Set_Time();
  Set_Alarm();


}

void Set_Time(){
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  //Initialise l'heure
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    //Error_Handler();
  }

  //Initialise le jour, le mois et l'année
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    // Erreur
  }
}

// (Re)initialise l'alarme en mode interruption à une minute
void Set_Alarm(){

  RTC_AlarmTypeDef sAlarm = {0};
  
  //Met l'alarme à 1 minute
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x1;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;

  //Paramétrage de l'alarme pour qu'elle s'exécute qu'une seule fois
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;

  //Application des différents paramètres sur l'alarme A
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    // Erreur
  }
}


void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&hrtc);
}

//FONCTION EXECUTEE LORS DE L'INTERRUPT
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef* hrtc){

  //Changement d'état de la led sur le STM32
	HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);

  //Re-initialise la rtc et l'alarme pour réexécuter cette fonction dans 1 minute
  Set_Time();
  Set_Alarm();
}


/*void readTemp_Send(){
  InitRcSwitch();
  enableTransmit();

  HAL_Delay(2000);

  uint16_t Temp = 0.0;
  uint16_t Hyd = 0.0;

  DHT22_GetTemp_Humidity(&Temp, &Hyd);

  send((uint32_t) Temp,24);
}*/
