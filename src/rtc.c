#include "rtc.h"


// INITIALISATION DU RTC
void RTC_Init()
{
  //Active la RTC
  __HAL_RCC_RTC_ENABLE();
  
  // Ajout de l'interruption pour l'alarme A
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

  //Calcul de asyncPrediv et de syncPrediv en fonction de la clock d'entrée de la STM32
  uint32_t quartzFrequency = 40000; // 40 kHz
  uint32_t asyncPrediv = 127; // Valeur par défaut
  uint32_t syncPrediv = quartzFrequency / (asyncPrediv+1)-1;

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = asyncPrediv;
  hrtc.Init.SynchPrediv = syncPrediv;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    LED_Loop(); //Erreur
  }

  //Inilialisation du temps et de l'alarme  
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
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    LED_Loop(); //Erreur
  }

  //Initialise le jour, le mois et l'année
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    LED_Loop(); //Erreur
  }
}

// (Re)initialise l'alarme en mode interruption à une minute
void Set_Alarm(){

  RTC_AlarmTypeDef sAlarm = {0};
  
  //Met l'alarme au temps choisi dans le header
  //Attention: a mettre en décimal et non en hexadécimal
  sAlarm.AlarmTime.Hours = HOURS;
  sAlarm.AlarmTime.Minutes = MINUTES;
  sAlarm.AlarmTime.Seconds = SECONDS;
  sAlarm.AlarmTime.SubSeconds = SUB_SECONDS;

  //Paramétrage de l'alarme pour qu'elle s'exécute qu'une seule fois
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;

  //Application des différents paramètres sur l'alarme A
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    LED_Loop();
  }
}


void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&hrtc);
}