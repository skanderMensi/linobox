#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include "src/PotTeensy/PotTeensy.h"
// #include "src/AudioEffect/AudioEffect.h"
// #include "src/AudioLooper/AudioLooper.h"
#include "src/AudioLooperDual/AudioLooperDual.h"
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
AudioLooperDual audioLooper;
unsigned long loopDuration1 = 0;
unsigned long loopDuration2 = 0;

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
  loopDuration1 = audioLooper.getLoopDuration(1);
  pad.set_loop_duration(1, loopDuration1);
  loopDuration2 = audioLooper.getLoopDuration(2);
  pad.set_loop_duration(2, loopDuration2);
  
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
  
  audioLooper.run();

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
      // LOOP 1
      case 0:
        if (falling){
          Serial.println("Record Button Pressed - LOOP 1");
          if (audioLooper.get_mode(1) == 2){
            pad.blink_loop_status(1, false);
            audioLooper.stopPlaying(1);
            }
          if (audioLooper.get_mode(1) == 0){
            audioLooper.startRecording(1);
            }
        }
        else{
          Serial.println("Record Button Released - LOOP 1");
          audioLooper.stopRecording(1);
          loopDuration1 = audioLooper.getLoopDuration(1);
          pad.set_loop_duration(1, loopDuration1);
          pad.blink_loop_status(1, true);
          audioLooper.startPlaying(1);
        }
        break;
      case 4:
        if (falling){
          Serial.println("Stop Button Press - LOOP 1");
          if (audioLooper.get_mode(1) == 0) {
            pad.blink_loop_status(1, true);
            audioLooper.startPlaying(1);
          }
          else if (audioLooper.get_mode(1) == 2) {
            pad.blink_loop_status(1, false);
            audioLooper.stopPlaying(1);
          }
        }
        break;
        // LOOP 2
        case 8:
        if (falling){
          Serial.println("Record Button Pressed - LOOP 2");
          if (audioLooper.get_mode(2) == 2){
            pad.blink_loop_status(2, false);
            audioLooper.stopPlaying(2);
            }
          if (audioLooper.get_mode(2) == 0){
            audioLooper.startRecording(2);
            }
        }
        else{
          Serial.println("Record Button Released - LOOP 2");
          audioLooper.stopRecording(2);
          loopDuration2 = audioLooper.getLoopDuration(2);
          pad.set_loop_duration(2, loopDuration2);
          pad.blink_loop_status(2, true);
          audioLooper.startPlaying(2);
        }
        break;
      case 12:
        if (falling){
          Serial.println("Stop Button Press - LOOP 2");
          if (audioLooper.get_mode(2) == 0) {
            pad.blink_loop_status(2, true);
            audioLooper.startPlaying(2);
          }
          else if (audioLooper.get_mode(2) == 2) {
            pad.blink_loop_status(2, false);
            audioLooper.stopPlaying(2);
          }
        }
        break;
      // DEFAULT
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
