/*
  GlobalInit.cpp - Global Variable Defintion for Rainbow Effect
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "GlobalInit.h"
#include <EEPROM.h>

////////////////////////////////////////////////
///////////// TRELLIS PARAMETERS ///////////////
////////////////////////////////////////////////
const uint8_t potentiometerPins[nbrPot] = {22, 15};
const uint8_t potThreshold = 5;
const uint8_t potMin = 0;
const uint8_t potMax = 1023;
const float parameterMin = 0.0;
const float parameterMax = 1.0;

////////////////////////////////////////////////
/////////////////// MODE ///////////////////////
////////////////////////////////////////////////
// https://hexcolor.co/pantone-colors
const uint32_t colorMic = 0xF50002;
const uint32_t colorVoice[nbrVoice] = {0x26C2FF, 0x40FFED, 0x4AFFAB};
const uint32_t colorEffect[nbrEffect] = {0xE39F40, 0x86B324, 0x1C778C, 0x3378FF};
const uint32_t colorSong[nbrSong] = {0xFF4040, 0xE3B122};

////////////////////////////////////////////////
////////////// SCALING FUNCTION ////////////////
////////////////////////////////////////////////

float map_int_to_float(int value, int intLow, int intHigh, float floatLow, float floatHigh){
  value = map(value, intLow, intHigh, int(floatLow * 1000), int(floatHigh * 1000));
  return float(value) / 1000.0;
}


void eeprom_clear_all(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

int eeprom_write_1byte(uint8_t value, int index){
  EEPROM.write(index, value);
  index++;
  return index;
}
