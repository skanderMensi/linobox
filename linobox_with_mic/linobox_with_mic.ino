#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include "src/PotTeensy/PotTeensy.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>

// AUDIO
// GUItool: begin automatically generated code
AudioInputI2S            input;
AudioSynthSimpleDrum     drum;
AudioSynthKarplusStrong  string;
AudioPlaySdWav           playSdWav;
AudioEffectMidSide       midSide;
AudioEffectBitcrusher    bitCrusher;
AudioEffectDelay         delayEffect;
AudioMixer4              inputMixer;
AudioEffectReverb        reverb;
AudioMixer4              effectMixer;
AudioMixer4              outputMixerL;
AudioMixer4              outputMixerR;
AudioOutputI2S           output;
AudioConnection          patchCord1(input, 0, inputMixer, 0);
AudioConnection          patchCord2(drum, 0, inputMixer, 1);
AudioConnection          patchCord3(string, 0, inputMixer, 2);
AudioConnection          patchCord4(playSdWav, 0, outputMixerL, 0);
AudioConnection          patchCord5(playSdWav, 0, midSide, 0);
AudioConnection          patchCord6(playSdWav, 1, outputMixerR, 0);
AudioConnection          patchCord7(playSdWav, 1, midSide, 1);
AudioConnection          patchCord8(midSide, 0, inputMixer, 3);
AudioConnection          patchCord9(bitCrusher, 0, effectMixer, 3);
AudioConnection          patchCord10(delayEffect, 0, effectMixer, 2);
AudioConnection          patchCord11(inputMixer, 0, effectMixer, 0);
AudioConnection          patchCord12(inputMixer, reverb);
AudioConnection          patchCord13(inputMixer, delayEffect);
AudioConnection          patchCord14(inputMixer, bitCrusher);
AudioConnection          patchCord15(reverb, 0, effectMixer, 1);
AudioConnection          patchCord16(effectMixer, 0, outputMixerL, 1);
AudioConnection          patchCord17(effectMixer, 0, outputMixerR, 1);
AudioConnection          patchCord18(outputMixerL, 0, output, 0);
AudioConnection          patchCord19(outputMixerR, 0, output, 1);
AudioControlSGTL5000     audioShield;
// GUItool: end automatically generated code

// TRELLIS PAD
TrellisCallback trellis_callback_wrapper(keyEvent evt);
PadTeensy pad;

// POTENTIOMETER
PotTeensy leftPot;
PotTeensy rightPot;

// GLOBAL VARIABLE
uint8_t voiceType = 0; // SD PLAYER
uint8_t effectType = 0; // NO EFFECT
uint8_t songType = 0; // FIRST PAGE
bool micStatus = false; // MIC OFF

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

  // INIT TRELLIS
  pad.init(trellis_callback_wrapper);

  // INIT POT
  leftPot.init(0);
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

  // INIT AUDIO OBJECT
  midSide.encode();
  bitCrusher.bits(12);
  bitCrusher.sampleRate(30000);
  delayEffect.delay(0, 500);
  for (int i=1;i<8;i++){
    delayEffect.disable(i);
  }
  reverb.reverbTime(2);
  for (int i=0;i<4;i++){
    inputMixer.gain(i, 0);
    effectMixer.gain(i, 0);
    outputMixerL.gain(i, 0);
    outputMixerR.gain(i, 0);
  }

  // INIT MIXER BASER ON MODE 0
  change_level(rightPot.get_parameter_value());

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
    Serial.println(leftPot.get_pot_value());
  }

  // RIGHT POTENTIOMETER
  if (rightPot.run_potentiometer()){
    Serial.print("rigth pot. = ");
    Serial.println(rightPot.get_pot_value());
    change_level(rightPot.get_pot_value());
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}

void change_level(float new_level){
  if (voiceType == 0){ // SD PLAYER
    outputMixerL.gain(0, new_level);
    outputMixerR.gain(0, new_level);
  }
  if (micStatus || (effectType > 0)){ // MIC AND / OR EFFECT
    outputMixerL.gain(1, new_level);
    outputMixerR.gain(1, new_level);
  }
}

void setMode(uint8_t message){

  switch (message) {

    case modeMic:
      if (micStatus){
        inputMixer.gain(0, defaultMicVol);
        effectMixer.gain(0, defaultDryVol);
        outputMixerL.gain(0, rightPot.get_parameter_value());
        outputMixerR.gain(0, rightPot.get_parameter_value());
        pad.set_color(modeMic, colorMic);
      }
      else{
        inputMixer.gain(0, 0);
        if (effectType == 0){
          effectMixer.gain(0, 0);
        }
        outputMixerL.gain(0, 0);
        outputMixerR.gain(0, 0);
        pad.set_color(modeMic, 0);
      }
      micStatus = !micStatus;
      break;

    case modeVoice:
      voiceType++; // CYCLE THROUGH EFFECTS
      if (voiceType > nbrVoice){
        voiceType = 0;
      }
      if (voiceType == 0){ // SD PLAYER
        inputMixer.gain(1, 0);
        inputMixer.gain(2, 0);
        if (effectType != 0){
          inputMixer.gain(3, defaultInputVol);
          effectMixer.gain(0, defaultDryVol);
          outputMixerL.gain(0, 0);
          outputMixerR.gain(0, 0);
          outputMixerL.gain(1, rightPot.get_parameter_value());
          outputMixerR.gain(1, rightPot.get_parameter_value());
        }
        else{
          inputMixer.gain(3, 0);
          effectMixer.gain(0, 0);
          outputMixerL.gain(0, rightPot.get_parameter_value());
          outputMixerR.gain(0, rightPot.get_parameter_value());
          outputMixerL.gain(1, 0);
          outputMixerR.gain(1, 0);
        }
      }
      else if (voiceType == 1){ // DRUM
        inputMixer.gain(1, defaultInputVol);
        inputMixer.gain(2, 0);
        inputMixer.gain(3, 0);
        effectMixer.gain(0, defaultDryVol);
        outputMixerL.gain(0, 0);
        outputMixerR.gain(0, 0);
        outputMixerL.gain(1, rightPot.get_parameter_value());
        outputMixerR.gain(1, rightPot.get_parameter_value());
      }
      else if (voiceType == 2){ // STRING
        inputMixer.gain(1, 0);
        inputMixer.gain(2, defaultInputVol);
        inputMixer.gain(3, 0);
        effectMixer.gain(0, defaultDryVol);
        outputMixerL.gain(0, 0);
        outputMixerR.gain(0, 0);
        outputMixerL.gain(1, rightPot.get_parameter_value());
        outputMixerR.gain(1, rightPot.get_parameter_value());
      }
      pad.set_color(modeVoice, colorVoice[voiceType]);
      break;

    case modeEffect:
      effectType++; // CYCLE THROUGH EFFECTS
      if (effectType > nbrEffect){
        effectType = 0;
      }
      if (effectType == 0){ // NO EFFECT
        reverb.reverbTime(0);
        effectMixer.gain(1, 0);
        delayEffect.disable(0);
        effectMixer.gain(2, 0);
        effectMixer.gain(3, 0);
      }
      else if (effectType == 1){ // REVERB
        reverb.reverbTime(5.0 * leftPot.get_parameter_value());
        effectMixer.gain(1, defaultWetVol);
      }
      else if (effectType == 2){ // DELAY
        reverb.reverbTime(0);
        effectMixer.gain(1, 0);
        delayEffect.gain(0, 500.0 * leftPot.get_parameter_value());
        effectMixer.gain(2, defaultWetVol);
      }
      else if (effectType == 3){ // BIT-CRUSHER
        delayEffect.disable(0);
        effectMixer.gain(2, 0);
        bitCrusher.bits(16 - int(16 * leftPot.get_parameter_value()));
        effectMixer.gain(3, defaultWetVol);
      }
      pad.set_color(modeEffect, colorEffect[effectType]);
      break;

    case modeSong:
      songType++; // CYCLE THROUGH EFFECTS
      if (songType > nbrSong){
        songType = 0;
      }
      pad.set_color(modeSong, colorSong[songType]);
      break;
  }

}
