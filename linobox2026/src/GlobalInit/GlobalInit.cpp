/*
  GlobalInit.cpp - Global Variable Defintion for Rainbow Effect
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "GlobalInit.h"
#include <EEPROM.h>

////////////////////////////////////////////////
////////////// DEFAULT VALUES //////////////////
////////////////////////////////////////////////

const uint8_t potentiometerPins[nbrPot] = {22, 15};
const int16_t potMin = 0;
const int16_t potMax = 1015;
// const uint32_t ledColors[nbrLed] = {0xF44336, 0xE81E63, 0x9C27B0, 0x673AB7,
//                                     0x3F51B5, 0x2196F3, 0x03A9F4, 0x00BCD4,
//                                     0x009688, 0x4CAF50, 0x8BC34A, 0xCDDC39,
//                                     0xFFEB3B, 0xFFC107, 0xFF9800, 0xFF5722};
const uint32_t ledColors[nbrLed] = {65280, 3198720, 6332160, 9465600,
                                    12599040, 15732480, 14549025, 11403345,
                                    8257665, 5308590, 2162910, 4080,
                                    16320, 28560, 40800, 53040};

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
