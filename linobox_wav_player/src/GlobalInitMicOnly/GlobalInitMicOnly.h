/*
  GlobalInit.h - Global Variable Defintion for Rainbow Effect
  Created by S. Mensi, September, 2022.
*/

#ifndef GlobalInitMicOnly_h
#define GlobalInitMicOnly_h

////////////////////////////////////////////////
////////////// SERIAL PARAMETERS ///////////////
////////////////////////////////////////////////
#define teensyBaudRate 9600

////////////////////////////////////////////////
///////////// TRELLIS PARAMETERS ///////////////
////////////////////////////////////////////////
#define trellisInterruptPin 2
#define nbrPot 2
extern const uint8_t potentiometerPins[nbrPot];
extern const uint8_t potThreshold;
extern const int16_t potMin;
extern const int16_t potMax;
extern const float parameterMin;
extern const float parameterMax;

////////////////////////////////////////////////
////////////// UTILITY FUNCTION ////////////////
////////////////////////////////////////////////
extern void eeprom_clear_all();
extern int eeprom_write_1byte(uint8_t value, int index);
extern float map_int_to_float(int value, int intLow, int intHigh, float floatLow, float floatHigh);


#endif
