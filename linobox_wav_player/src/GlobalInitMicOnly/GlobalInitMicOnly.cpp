/*
  GlobalInit.cpp - Global Variable Defintion for Rainbow Effect
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "GlobalInitMicOnly.h"
#include <EEPROM.h>

////////////////////////////////////////////////
///////////// TRELLIS PARAMETERS ///////////////
////////////////////////////////////////////////
const uint8_t potentiometerPins[nbrPot] = {22, 15};
const uint8_t potThreshold = 5;
const int16_t potMin = 0;
const int16_t potMax = 1023;
const float parameterMin = 0.0;
const float parameterMax = 1.0;

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
