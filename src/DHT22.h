//Header files
#include "main.h"

#define     READ_PIN          HAL_GPIO_ReadPin(GPIO_DHT22, PIN_DHT22)


//Change le sens du pin 0 -> entree , 1-> sortie
static void ChangePinMode(uint8_t mode);
//Envoie un signal au DHT22 pour ensuite recevoir les valeurs
static void DHT22_StartAcquisition(void);
//Lit les 5 octets (40bits) de valeur
static void DHT22_ReadRaw(uint8_t *data);
//Fonction pour récupérer la température et l'humidité ( retourne 1 si tout est correct sinon 0 )
uint8_t DHT22_GetTemp_Humidity(uint16_t *Temp, uint16_t *Humidity);