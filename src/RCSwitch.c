/*
  RCSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.
  
  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  - Andreas Steinel / A.<lastname>(at)gmail(dot)com
  - Max Horn / max(at)quendi(dot)de
  - Robert ter Vehn / <first name>.<last name>(at)gmail(dot)com
  - Johann Richard / <first name>.<last name>(at)gmail(dot)com
  - Vlad Gheorghe / <first name>.<last name>(at)gmail(dot)com https://github.com/vgheo
  - Matias Cuenca-Acuna 
  
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

#include "RCSwitch.h"
#include "dwt_delay.h"


#define RECEIVE_ATTR
#define VAR_ISR_ATTR

    


/* Format for protocol definitions:
 * {pulselength, Sync bit, "0" bit, "1" bit, invertedSignal}
 * 
 * pulselength: pulse length in microseconds, e.g. 350
 * Sync bit: {1, 31} means 1 high pulse and 31 low pulses
 *     (perceived as a 31*pulselength long pulse, total length of sync bit is
 *     32*pulselength microseconds), i.e:
 *      _
 *     | |_______________________________ (don't count the vertical bars)
 * "0" bit: waveform for a data bit of value "0", {1, 3} means 1 high pulse
 *     and 3 low pulses, total length (1+3)*pulselength, i.e:
 *      _
 *     | |___
 * "1" bit: waveform for a data bit of value "1", e.g. {3,1}:
 *      ___
 *     |   |_
 *
 * These are combined to form Tri-State bits when sending or receiving codes.
 */


static const VAR_ISR_ATTR Protocol proto[] = {
  { 350, {  1, 31 }, {  1,  3 }, {  3,  1 }, 0 },    // protocol 1
  { 650, {  1, 10 }, {  1,  2 }, {  2,  1 }, 0 },    // protocol 2
  { 100, { 30, 71 }, {  4, 11 }, {  9,  6 }, 0 },    // protocol 3
  { 380, {  1,  6 }, {  1,  3 }, {  3,  1 }, 0 },    // protocol 4
  { 500, {  6, 14 }, {  1,  2 }, {  2,  1 }, 0 },    // protocol 5
  { 450, { 23,  1 }, {  1,  2 }, {  2,  1 }, 1 },     // protocol 6 (HT6P20B)
  { 150, {  2, 62 }, {  1,  6 }, {  6,  1 }, 0 },    // protocol 7 (HS2303-PT, i. e. used in AUKEY Remote)
  { 200, {  3, 130}, {  7, 16 }, {  3,  16}, 0},     // protocol 8 Conrad RS-200 RX
  { 200, { 130, 7 }, {  16, 7 }, { 16,  3 }, 1},      // protocol 9 Conrad RS-200 TX
  { 365, { 18,  1 }, {  3,  1 }, {  1,  3 }, 1 },     // protocol 10 (1ByOne Doorbell)
  { 270, { 36,  1 }, {  1,  2 }, {  2,  1 }, 1 },     // protocol 11 (HT12E)
  { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, 1 }      // protocol 12 (SM5212)
};

enum {
   numProto = sizeof(proto) / sizeof(proto[0])
};

void InitRcSwitch() {

  DWT_Init();

  setRepeatTransmit(4);
  setProtocolByNum(1);

}

/**
  * Sets the protocol to send.
  */
void setProtocol(Protocol pProtocol) {
  protocol = pProtocol;
}

/**
  * Sets the protocol to send, from a list of predefined protocols
  */
void setProtocolByNum(int nProtocol) {
  if (nProtocol < 1 || nProtocol > numProto) {
    nProtocol = 1;
  }
  protocol = proto[nProtocol-1];
}

/**
  * Sets the protocol to send with pulse length in microseconds.
  */
void setProtocolByNumLength(int nProtocol, int nPulseLength) {
  setProtocolByNum(nProtocol);
  setPulseLength(nPulseLength);
}


/**
  * Sets pulse length in microseconds
  */
void setPulseLength(int nPulseLength) {
  protocol.pulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 */
void setRepeatTransmit(int nRepeat) {
  nRepeatTransmit = nRepeat;
}
  

/**
 * Enable transmissions
 */
void enableTransmit() {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_RF;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIO_RF, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIO_RF, GPIO_PIN_RF,1);

}

/**
 * send character
 */
void sendChar(const char* sCodeWord) {
  // turn the tristate code word into the corresponding bit pattern, then send it
  unsigned long code = 0;
  unsigned int length = 0;
  for (const char* p = sCodeWord; *p; p++) {
    code <<= 1L;
    if (*p != '0')
      code |= 1L;
    length++;
  }
  send(code, length);
}

/**
 * Transmit the first 'length' bits of the integer 'code'. The
 * bits are sent from MSB to LSB, i.e., first the bit at position length-1,
 * then the bit at position length-2, and so on, till finally the bit at position 0.
 */
void send(unsigned long code, unsigned int length) {
  //if (nTransmitterPin == -1) return;

  for (int nRepeat = 0; nRepeat < nRepeatTransmit; nRepeat++) {
    for (int i = length-1; i >= 0; i--) {
      if (code & (1L << i))
        transmit(protocol.one);
      else
        transmit(protocol.zero);
    }
    transmit(protocol.syncFactor);
  }

  // Disable transmit after sending (i.e., for inverted protocols)
  //HAL_GPIO_WritePin(GPIO_RF, GPIO_PIN_RF, LOW);

}

/**
 * Transmit a single high-low pulse.
 */
static void transmit(HighLow pulses) {
  GPIO_PinState firstLogicLevel   = (protocol.invertedSignal) ? LOW   : HIGH;
  GPIO_PinState secondLogicLevel  = (protocol.invertedSignal) ? HIGH  : LOW;

  HAL_GPIO_WritePin(GPIO_RF, GPIO_PIN_RF,firstLogicLevel);
  DWT_Delay(pulses.high*protocol.pulseLength);
  
  HAL_GPIO_WritePin(GPIO_RF, GPIO_PIN_RF,secondLogicLevel);
  DWT_Delay(pulses.low*protocol.pulseLength);
  
}


