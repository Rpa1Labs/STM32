/**
 * @file ControlLED.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Simple bout de code pour faire clignotter la LED intégrée au STM32
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main.h"

#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

/**
 * @brief Fonction pour initialiser le PIN de la LED intégrée
 * 
 */
void LED_Init();

/**
 * @brief Fonction pour faire clignotter la LED à l'infini
 * 
 */
void LED_Loop();