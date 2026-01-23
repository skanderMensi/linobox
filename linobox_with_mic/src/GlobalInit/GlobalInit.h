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
////////////// DEFAULT VOLUME //////////////////
////////////////////////////////////////////////
extern const float defaultMicVol = 0.5;
extern const float defaultDryVol = 0.5;
extern const float defaultWetVol = 0.5;
extern const float defaultInputVol = 0.5;

////////////////////////////////////////////////
/////////////////// MODE ///////////////////////
////////////////////////////////////////////////
// https://hexcolor.co/pantone-colors
#define modeMic 0
#define modeVoice 1
#define modeEffect 2
#define modeSong 3

#define nbrVoice 3
#define nbrEffect 4
#define nbrSong 2
extern const uint32_t colorMic;
extern const uint32_t colorVoice[nbrVoice];
extern const uint32_t colorEffect[nbrEffect];
extern const uint32_t colorSong[nbrSong];

////////////////////////////////////////////////
///////////// TRELLIS PARAMETERS ///////////////
////////////////////////////////////////////////
#define trellisInterruptPin 2
#define nbrPot 2
extern const uint8_t potentiometerPins[nbrPot];
extern const uint8_t potThreshold;
extern const uint8_t potMin;
extern const uint8_t potMax;
extern const float parameterMin;
extern const float parameterMax;

////////////////////////////////////////////////
////////////// UTILITY FUNCTION ////////////////
////////////////////////////////////////////////
extern void eeprom_clear_all();
extern int eeprom_write_1byte(uint8_t value, int index);
extern float map_int_to_float(int value, int intLow, int intHigh, float floatLow, float floatHigh);


#endif
