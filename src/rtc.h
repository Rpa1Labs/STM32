/**
 * @file rtc.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Librairie permettant d'utiliser l'interruption de la rtc de la STM32
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//Headers
#include "main.h"
#include "ControlLED.h"

RTC_HandleTypeDef hrtc;
#define RTC_INTERRUPT() HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef* hrtc)


/**
 * @brief Initialisation de la rtc interne à la STM32
 * 
 */
void RTC_Init(void);

/**
 * @brief (Re)met le temps à 0 sur la rtc
 * 
 */
void Set_Time();

/**
 * @brief (Re)met l'alarme au temps choisi ci-dessous
 * 
 */
void Set_Alarm();


/** TEMPS ENTRE CHAQUE INTERRUPTION */

#define SUB_SECONDS     0
#define SECONDS         10
#define MINUTES         0
#define HOURS           0
