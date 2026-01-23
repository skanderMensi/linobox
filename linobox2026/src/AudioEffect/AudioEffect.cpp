/*
  PotTeensy.cpp - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "AudioEffect.h"
#include "../GlobalInit/GlobalInit.h"


AudioEffect::AudioEffect(){
    // GUItool: begin automatically generated code
    _i2s1 = new AudioInputI2S;
    _inputAmp = new AudioAmplifier;
    _queue = new AudioRecordQueue;
    _playSdRaw = new AudioPlaySdRaw;
    _inputMixer = new AudioMixer4;
    _i2s2 = new AudioOutputI2S;

    _sine = new AudioSynthWaveformSine;

    _patchCord1 = new AudioConnection;  //(_i2s1, 0, _inputAmp, 0); 
    _patchCord2 = new AudioConnection;  //(_inputAmp, 0, _inputMixer, 0);
    _patchCord3 = new AudioConnection;  //(_inputAmp, _queue);
    _patchCord4 = new AudioConnection;  //(_playSdRaw, 0, _inputMixer, 1);
    _patchCord5 = new AudioConnection;  //(_inputMixer, 0, _i2s2, 0);
    _patchCord6 = new AudioConnection;  //(_inputMixer, 0, _i2s2, 1);
    
    _patchCord7 = new AudioConnection;  //(_inputMixer, 0, _i2s2, 1);
    
    _audioShield = new AudioControlSGTL5000;
    // GUItool: end automatically generated code
}

void AudioEffect::init(){
    // CONNECT
    _patchCord1->connect(*_i2s1, 0, *_inputAmp, 0); 
    _patchCord2->connect(*_inputAmp, 0, *_inputMixer, 0);
    _patchCord3->connect(*_inputAmp, *_queue);
    _patchCord4->connect(*_playSdRaw, 0, *_inputMixer, 1);
    _patchCord5->connect(*_inputMixer, 0, *_i2s2, 0);
    _patchCord6->connect(*_inputMixer, 0, *_i2s2, 1);

    _patchCord7->connect(*_sine, 0, *_inputMixer, 2);

    // INIT AUDIO SHIELD
    AudioMemory(15);
    _audioShield->enable();
    _audioShield->inputSelect(AUDIO_INPUT_MIC);
    _audioShield->audioPostProcessorEnable();
    _audioShield->eqSelect(2);
    _audioShield->volume(0.6);

    // INIT MIXER BASER ON MODE 0
    _inputAmp->gain(1.0);
    _audioShield->micGain(10);
    _inputMixer->gain(0, 1);
    _inputMixer->gain(1, 0);
    _inputMixer->gain(2, 0.5);
    _inputMixer->gain(3, 0);
    
    // INIT SINE WAVE
    _sine->frequency(440);
    _sine->amplitude(0.8);
    
    // End INIT
    Serial.println("Audio Shield initialized");
}


void AudioEffect::set_frequency(int freq){
    _sine->frequency(freq);
}

void AudioEffect::set_volume(float vol){
    _audioShield->volume(vol);
}

void AudioEffect::run(){

}
