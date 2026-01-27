#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include "src/PotTeensy/PotTeensy.h"
// #include "src/AudioEffect/AudioEffect.h"
#include "src/AudioLooper/AudioLooper.h"
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

// // AUDIO EFFECT
// AudioEffect audioEffect;

// AUDIO LOOPER
AudioLooper audioLooper;

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
  Serial.println("NeoTrellis Initialized");

  // init audio
  audioLooper.init();

  // init potentiometers
  potVol.init(1);
  volume = potVol.get_parameter();
  audioLooper.set_volume(volume);   // HeadPhones
  
  // potParam.init(0);
  // param = potParam.get_parameter();
  // audioEffect.set_frequency(map(int(param * 100), 0, 100, 0, 15000));

  // Init done
  previousMillis = millis();
}

// MAIN LOOP
void loop() {
  uint16_t message;
  
  if (pad.run()){
    message = pad.get_message();
    pad.reset_message();
    Serial.print("message from trellis: ");
    Serial.println(message);
    bool falling = false;
    if (message >= 1000){
      message = message - 1000;
      falling = true;
    }
    switch(message){
      case 0:
        if (falling){
          Serial.println("Record Button Pressed");
          if (audioLooper.get_mode() == 2){
            audioLooper.stopPlaying();
            }
          if (audioLooper.get_mode() == 0){
            audioLooper.startRecording();
            }
        }
        else{
          Serial.println("Record Button Released");
          audioLooper.stopRecording();
          audioLooper.startPlaying();
        }
        break;
      case 4:
        if (falling){
          Serial.println("Stop Button Press");
          if (audioLooper.get_mode() == 0) {
            audioLooper.startPlaying();
          }
          else if (audioLooper.get_mode() == 2) {
            audioLooper.stopPlaying();
          }
        }
        break;
      default:
        if (falling){
            Serial.print("play note @(Hz): ");
            Serial.println(message * 220);
            audioLooper.noteOn(message * 220);
        }
        break;  
    }
    
  }

  // VOLUME POTENTIOMETER
  if (potVol.run()){
    volume = potVol.get_parameter();
    audioLooper.set_volume(volume);   // HeadPhones

    Serial.print("volume = ");
    Serial.println(volume);
  }
  // // PARAM POTENTIOMETER
  // if (potParam.run()){
  //   param = potParam.get_parameter();
  //   Serial.print("parameters = ");
  //   Serial.println(param);
    
  //   audioEffect.set_frequency(map(int(param * 100), 0, 100, 0, 15000));
  // }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}
