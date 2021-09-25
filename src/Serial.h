//#include <stm32f3xx_hal.h>
#include "main.h"

#define Serial                           UART_HandleTypeDef

/**
 * LIBRAIRIE POUR SIMPLIFIER LA COMMUNICATION UART SUR LE STM32 
 * ATTENTION: VALABLE UNIQUEMENT SUR LE GPIOA
 * 
 * Setup:
 *  - Créer une variable de type "Serial" en dehors de toute fonction ( oui c'est dégeulasse )
 *  - Initialiser la communication avec Init( adresse de la variable serial, baudrate, pin du TX, pin du RX )
 * 
 * Utilisation:
 *  - Send( adresse de la variable serial, adresse de la donnée, taille en octets ) -> pour envoyer des données
 *  - Receive ( adresse de la variable serial, adresse de la donnée, taille en octets, temps max pour récupérer les données en ms ) -> pour récupérer des données
*/

void Init(Serial *self, int baudRate, uint16_t PIN_Tx, uint16_t PIN_Rx);
void Send(Serial *self, uint8_t* data,int numberOfBytes);
void Receive(Serial* self, uint8_t* data, int sizeInBytes, int timeOut);
void End();