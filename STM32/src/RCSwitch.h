/**
 * @file RCSwitch.h
 * @author Rpa1Labs (romain.pajon@etu.univ-orleans.fr)
 * @brief Librairie allegée pour la transmission en 433Mhz pour la STM32, basée sur le projet: https://github.com/sui77/rc-switch/
 * @version 1.0
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "main.h"

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

/**
 * @brief Number of maximum high/Low changes per packet. We can handle up to (unsigned long) => 32 bit * 2 H/L changes per bit + 2 for sync
 * 
 */
#define RCSWITCH_MAX_CHANGES 67

/**
     * Description of a single pule, which consists of a high signal
     * whose duration is "high" times the base pulse length, followed
     * by a low signal lasting "low" times the base pulse length.
     * Thus, the pulse overall lasts (high+low)*pulseLength
     */

typedef struct{
    uint8_t high;
    uint8_t low;
} HighLow;

/**
     * A "protocol" describes how zero and one bits are encoded into high/low
     * pulses.
     */

/** base pulse length in microseconds, e.g. 350 */
typedef struct{
    uint16_t pulseLength;
    HighLow syncFactor;
    HighLow zero;
    HighLow one;
    uint8_t invertedSignal;
} Protocol;

/**
         * If true, interchange high and low logic levels in all transmissions.
         *
         * By default, RCSwitch assumes that any signals it sends or receives
         * can be broken down into pulses which start with a high signal level,
         * followed by a a low signal level. This is e.g. the case for the
         * popular PT 2260 encoder chip, and thus many switches out there.
         *
         * But some devices do it the other way around, and start with a low
         * signal level, followed by a high signal level, e.g. the HT6P20B. To
         * accommodate this, one can set invertedSignal to true, which causes
         * RCSwitch to change how it interprets any HighLow struct FOO: It will
         * then assume transmissions start with a low signal lasting
         * FOO.high*pulseLength microseconds, followed by a high signal lasting
         * FOO.low*pulseLength microseconds.
    */



/**
 * @brief Fonction pour initialiser le PIN + le protocol et le nombre de répétitions par défaut
 * 
 */
void InitRcSwitch();


/**
 * @brief Fonction pour envoyer du binaire au module RF 433Mhz
 * 
 * @param code  Données à envoyer
 * @param length Longueur en bit des données
 */
void send(unsigned long code, unsigned int length);

/**
 * @brief Initialise le PIN pour la transmission RF
 * 
 */
void enableTransmit();

/**
 * @brief Dé-initialise le PIN pour la transmission RF
 * 
 */
void disableTransmit();

/**
 * @brief Transmet 1 bit en fonction du protocole au module RF
 * 
 * @param pulses pulses.one ou pulses.zero
 */
void transmit(HighLow pulses);

/**
 * @brief Fonction pour choisir le protocole en fonction de son numéro
 * 
 * @param nProtocol Numéro du protocol
 */
void setProtocolByNum(int nProtocol);

/**
 * @brief Fonction pour choisir le nombre de fois que le signal sera envoyé
 * 
 * @param nRepeatTransmit Nombre de fois
 */
void setRepeatTransmit(int nRepeatTransmit);

int nRepeatTransmit;
Protocol protocol;
