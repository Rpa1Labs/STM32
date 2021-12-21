/**
 * @file DHT22.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Librairie permettant de récupérer la température et l'humidité d'un DHT22 pour la STM32
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//Header
#include "main.h"

#define     READ_PIN          HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22)

/**
 * @brief Change le sens du pin 
 * 
 * @param mode 0 -> entree , 1-> sortie
 */
void ChangePinMode(uint8_t mode);

/**
 * @brief Envoie un signal au DHT22 pour ensuite recevoir les valeurs
 * 
 */
void DHT22_StartAcquisition(void);

/**
 * @brief Lit les 5 octets (40bits) envoyés par le DHT22
 * 
 * @param data Adresse pointant vers un tableau de 5 octets
 */
void DHT22_ReadRaw(uint8_t *data);

/**
 * @brief Fonction pour récupérer la température et l'humidité
 * 
 * @param Temp Adresse pointant sur un uint16_t contenant la température
 * @param Humidity Adresse pointant sur un uint16_t contenant l'humidité
 * @return uint8_t retourne 1 si tout est correct sinon 0
 */
uint8_t DHT22_GetTemp_Humidity(uint16_t *Temp, uint16_t *Humidity);