#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include "src/PotTeensy/PotTeensy.h"
#include "src/AudioEffect/AudioEffect.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>


// TRELLIS PAD
TrellisCallback trellis_callback_wrapper(keyEvent evt);
PadTeensy pad;

// GLOBAL VARIABLE
float volume, param;
PotTeensy potVol;
PotTeensy potParam;

// AUDIO EFFECT
AudioEffect audioEffect;

// TIMING
unsigned long previousMillis;

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

  // INIT RANDOM SEED WITH POSITION OF THE POTENTIOMETER
  int random_seed = analogRead(volPin);
  randomSeed(random_seed);
  Serial.print("Random seed set to: ");
  Serial.println(random_seed);

  // init trellis pad
  pad.init(trellis_callback_wrapper);

  // init audio
  audioEffect.init();

  // init potentiometers
  potVol.init(0);
  volume = potVol.get_parameter();
  audioEffect.set_volume(volume);
  
  potParam.init(1);
  param = potParam.get_parameter();
  audioEffect.set_frequency(map(int(param * 100), 0, 100, 0, 15000));

  // Init done
  previousMillis = millis();
}

// MAIN LOOP
void loop() {
  uint8_t message;
  
  if (pad.run()){
    message = pad.get_message();
    pad.reset_message();
    Serial.print("message from trellis: ");
    Serial.println(message);
  }

  // VOLUME POTENTIOMETER
  if (potVol.run()){
    volume = potVol.get_parameter();
    audioEffect.set_volume(volume);

    Serial.print("volume = ");
    Serial.println(volume);
  }
  // PARAM POTENTIOMETER
  if (potParam.run()){
    param = potParam.get_parameter();
    audioEffect.set_frequency(map(int(param * 100), 0, 100, 0, 15000));
    
    Serial.print("parameters = ");
    Serial.println(param);
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}
