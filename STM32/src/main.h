/**
 * @file main.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Programme permettant d'envoyer via du 433Mhz la température d'un capteur DHT22
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//Header
#include "stm32f3xx_hal.h"

void readTemp_Send();

/** DHT22 */
#define     GPIO_DHT22          GPIOA
#define     PIN_DHT22           GPIO_PIN_9

/** RCSwitch */
#define     GPIO_RF             GPIOB
#define     GPIO_PIN_RF         GPIO_PIN_10

/** RCSwitch header */
#define     HEADER              73

/** LED */
#define     LED_PIN             GPIO_PIN_5
#define     LED_GPIO_PORT       GPIOA