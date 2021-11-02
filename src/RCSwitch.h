/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.
  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  - Max Horn / max(at)quendi(dot)de
  - Robert ter Vehn / <first name>.<last name>(at)gmail(dot)com
  
  Project home: https://github.com/sui77/rc-switch/
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdint.h>
#include <string.h> // memcpy
#include <stdlib.h> // abs

#include "main.h"

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

// Number of maximum high/Low changes per packet.
// We can handle up to (unsigned long) => 32 bit * 2 H/L changes per bit + 2 for sync
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

//public
void InitRcSwitch();

void send(unsigned long code, unsigned int length);
void sendChar(const char *sCodeWord);

void enableTransmit();
void disableTransmit();
void setPulseLength(int nPulseLength);
void setRepeatTransmit(int nRepeatTransmit);

void setProtocol(Protocol protocol);
void setProtocolByNum(int nProtocol);
void setProtocolByNumLength(int nProtocol, int nPulseLength);

//private
static void transmit(HighLow pulses);

int nRepeatTransmit;
Protocol protocol;
