/*
  GlobalInit.h - Global Variable Defintion for Rainbow Effect
  Created by S. Mensi, September, 2022.
*/

#ifndef GlobalInit_h
#define GlobalInit_h

////////////////////////////////////////////////
////////////// SERIAL PARAMETERS ///////////////
////////////////////////////////////////////////
#define teensyBaudRate 9600

////////////////////////////////////////////////
///////////// TRELLIS PARAMETERS ///////////////
////////////////////////////////////////////////
#define trellisInterruptPin 2
#define volPin 15
#define potPin 22
#define nbrPot 2
extern const uint8_t potentiometerPins[nbrPot];
extern const int16_t potMin;
extern const int16_t potMax;
#define nbrLed 16
extern const uint32_t ledColors[nbrLed];

////////////////////////////////////////////////
//// SETUP SD CARD ON TEENSY AUDIO SHIELD //////
////////////////////////////////////////////////
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13

////////////////////////////////////////////////
////////////// UTILITY FUNCTION ////////////////
////////////////////////////////////////////////
extern void eeprom_clear_all();
extern int eeprom_write_1byte(uint8_t value, int index);
extern float map_int_to_float(int value, int intLow, int intHigh, float floatLow, float floatHigh);


#endif
