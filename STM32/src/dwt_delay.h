/**
 * @file dwt_delay.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Petite librairie permettant de faire des délais en microsecondes, basée sur : https://github.com/keatis/dwt_delay
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>

/**
 * @brief Fonction pour initialiser le timer
 * 
 */
void DWT_Init(void);

/**
 * @brief Fonction pour effecture un délai
 * 
 * @param us Temps en microsecondes
 */
void DWT_Delay(uint32_t us);
