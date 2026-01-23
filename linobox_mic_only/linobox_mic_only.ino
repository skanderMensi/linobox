#include "src/GlobalInitMicOnly/GlobalInitMicOnly.h"
#include "src/PadTeensyMicOnly/PadTeensyMicOnly.h"
#include "src/PotTeensyMicOnly/PotTeensyMicOnly.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            input;           //xy=57,45
AudioAnalyzePeak         peak;          //xy=383,120
AudioMixer4              mixer;         //xy=385,58
AudioOutputI2S           output;           //xy=651,43
AudioConnection          patchCord1(input, 0, peak, 0);
AudioConnection          patchCord2(input, 0, mixer, 0);
AudioConnection          patchCord3(mixer, 0, output, 0);
AudioConnection          patchCord4(mixer, 0, output, 1);
AudioControlSGTL5000     audioShield;     //xy=82,544
// GUItool: end automatically generated code

// TRELLIS PAD
TrellisCallback trellis_callback_wrapper(keyEvent evt);
PadTeensyMicOnly pad;

// POTENTIOMETER
PotTeensyMicOnly leftPot;
PotTeensyMicOnly rightPot;

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

  // INIT TRELLIS
  pad.init(trellis_callback_wrapper);

  // INIT POT
  leftPot.init(0);
  leftPot.set_parameter_range(0, 63);
  rightPot.init(1);

  // INIT RANDOM SEED WITH POSITION OF THE POTENTIOMETER
  randomSeed(leftPot.get_pot_value() ^ rightPot.get_pot_value());

  // INIT AUDIO SHIELD
  AudioMemory(15);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.audioPostProcessorEnable();
  audioShield.eqSelect(2);
  audioShield.volume(0.6);

  // INIT MIXER BASER ON MODE 0
  mixer.gain(0, rightPot.get_parameter_value());
  audioShield.micGain(int(leftPot.get_parameter_value()));
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

  // LEFT POTENTIOMETER
  if (leftPot.run_potentiometer()){
    Serial.print("left pot. = ");
    Serial.println(leftPot.get_parameter_value());
    audioShield.micGain(int(leftPot.get_parameter_value()));
  }

  // RIGHT POTENTIOMETER
  if (rightPot.run_potentiometer()){
    Serial.print("rigth pot. = ");
    Serial.println(rightPot.get_parameter_value());
    mixer.gain(0, rightPot.get_parameter_value());
  }

  // PEAK
  if (peak.available()){
    Serial.print("New Peak Value = ");
    Serial.println(peak.read());
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}
