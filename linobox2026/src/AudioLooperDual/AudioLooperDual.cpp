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
#include "AudioLooperDual.h"
#include "../GlobalInit/GlobalInit.h"


AudioLooperDual::AudioLooperDual(){
    // GUItool: begin automatically generated code
    _i2s1 = new AudioInputI2S;
    _inputAmp = new AudioAmplifier;
    _stringSynth = new AudioSynthKarplusStrong;
    _inputMixer = new AudioMixer4;
    _queue1 = new AudioRecordQueue;
    _queue2 = new AudioRecordQueue;
    _playSdRaw1 = new AudioPlaySdRaw;
    _playSdRaw2 = new AudioPlaySdRaw;
    _outputMixer = new AudioMixer4;
    _i2s2 = new AudioOutputI2S;

    _patchCord1 = new AudioConnection;  //(i2s1, 0, _inputAmp, 0);
    _patchCord2 = new AudioConnection;  //(_inputAmp, 0, _inputMixer, 0);
    _patchCord3 = new AudioConnection;  //(_stringSynth, 0, _inputMixer, 1);
    _patchCord4 = new AudioConnection;  //(_inputMixer, _queue1);
    _patchCord5 = new AudioConnection;  //(_inputMixer, 0, _outputMixer, 0);
    _patchCord6 = new AudioConnection;  //(_playSdRaw1, 0, _outputMixer, 1);
    _patchCord7 = new AudioConnection;  //(_outputMixer, 0, _i2s2, 0);
    _patchCord8 = new AudioConnection;  //(_outputMixer, 0, _i2s2, 1);
    _patchCord9 = new AudioConnection;  //(_inputMixer, _queue2);
    _patchCord10 = new AudioConnection;  //(_playSdRaw2, 0, _outputMixer, 2);
    
    _audioShield = new AudioControlSGTL5000;
    // GUItool: end automatically generated code
}

void AudioLooperDual::init(){

    Serial.println("Init looper");
    // CONNECT
    _patchCord1->connect(*_i2s1, 0, *_inputAmp, 0); 
    _patchCord2->connect(*_inputAmp, 0, *_inputMixer, 0);
    _patchCord3->connect(*_stringSynth, 0, *_inputMixer, 1);
    _patchCord4->connect(*_inputMixer, *_queue1);
    _patchCord5->connect(*_inputMixer, 0, *_outputMixer, 0);
    _patchCord6->connect(*_playSdRaw1, 0, *_outputMixer, 1);
    _patchCord7->connect(*_outputMixer, 0, *_i2s2, 0);
    _patchCord8->connect(*_outputMixer, 0, *_i2s2, 1);
    _patchCord9->connect(*_inputMixer, *_queue2);
    _patchCord10->connect(*_playSdRaw2, 0, *_outputMixer, 2);

    // INIT AUDIO SHIELD
    AudioMemory(512);
    _audioShield->enable();
    _audioShield->inputSelect(AUDIO_INPUT_MIC);
    _audioShield->audioPostProcessorEnable();
    _audioShield->micGain(20);
    _audioShield->eqSelect(2);
    _audioShield->volume(0.8);
    _audioShield->dacVolumeRamp();
    _audioShield->dacVolume(0.8);

    // INIT INPUT AMPLIFIER
    _inputAmp->gain(1.0);

    // INIT INPUT MIXER
    _inputMixer->gain(0, 0.5);
    _inputMixer->gain(1, 0.5);
    _inputMixer->gain(2, 0);
    _inputMixer->gain(3, 0);
    
    // INIT OUTPUT MIXER
    _outputMixer->gain(0, 0.5);
    _outputMixer->gain(1, 0.5);
    _outputMixer->gain(2, 0.5);
    _outputMixer->gain(3, 0);

    // Initialize the SD card
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) { // stop here if no SD card, but print a message
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }  
    
    // LOOP TEMPO - 1
    continuePlaying(1);
    delay(200);
    stopPlaying(1);

    // LOOP TEMPO - 2
    continuePlaying(2);
    delay(200);
    stopPlaying(2);


    // End INIT
    Serial.println("Audio Shield initialized");
}


void AudioLooperDual::noteOn(int freq){
    _stringSynth->noteOn(freq, 0.8);
    _note_is_playing = true;
    _previousMillis = millis();
}

void AudioLooperDual::noteOff(){
    if (millis() - _previousMillis > _noteDurationMillis){
    _stringSynth->noteOff(0.8);
    _note_is_playing = false;
    }
}

void AudioLooperDual::set_volume(float vol){
    _audioShield->volume(vol);
    _audioShield->dacVolume(vol);
}

void AudioLooperDual::run(){
    if (_note_is_playing){
        this->noteOff();
    }
    
    // If LOOP 1 is playing or recording, carry on...
    if (_mode1 == 1) {this->continueRecording(1);}
    if (_mode1 == 2) {this->continuePlaying(1);}

    // If LOOP 2 playing or recording, carry on...
    if (_mode2 == 1) {this->continueRecording(2);}
    if (_mode2 == 2) {this->continuePlaying(2);}

}

//////////////////////////////////////////////
////////////// UTILITY FUNCTIONS /////////////
//////////////////////////////////////////////

int AudioLooperDual::get_mode(int loopID){
  if (loopID == 1){
    return _mode1;
  }
  else if (loopID == 2){
    return _mode2;
  }
  return false;
}

//////////////////////////////////////////////
// FUNCTION TO RECORD AND PLAY FROM SD CARD //
//////////////////////////////////////////////

void AudioLooperDual::startRecording(int loopID) {
  if (loopID == 1){
    Serial.println("startRecording LOOP 1");
    if (SD.exists("RECORD_1.RAW")) {
      // The SD library writes new data to the end of the
      // file, so to start a new recording, the old file
      // must be deleted before new data is written.
      SD.remove("RECORD_1.RAW");
    }
    frec1 = SD.open("RECORD_1.RAW", FILE_WRITE);
    if (frec1) {
      _queue1->begin();
      _mode1 = 1;
    }
  }
  else if (loopID == 2){
    Serial.println("startRecording LOOP 2");
    if (SD.exists("RECORD_2.RAW")) {
      // The SD library writes new data to the end of the
      // file, so to start a new recording, the old file
      // must be deleted before new data is written.
      SD.remove("RECORD_2.RAW");
    }
    frec2 = SD.open("RECORD_2.RAW", FILE_WRITE);
    if (frec2) {
      _queue2->begin();
      _mode2 = 1;
    }
  }
}

void AudioLooperDual::continueRecording(int loopID) {
  if (loopID == 1){
    if (_queue1->available() >= 2) {
      byte buffer[512];
      // Fetch 2 blocks from the audio library and copy
      // into a 512 byte buffer.  The Arduino SD library
      // is most efficient when full 512 byte sector size
      // writes are used.
      memcpy(buffer, _queue1->readBuffer(), 256);
      _queue1->freeBuffer();
      memcpy(buffer+256, _queue1->readBuffer(), 256);
      _queue1->freeBuffer();
      // write all 512 bytes to the SD card
      // elapsedMicros usec = 0;
      frec1.write(buffer, 512);
      // Uncomment these lines to see how long SD writes
      // are taking.  A pair of audio blocks arrives every
      // 5802 microseconds, so hopefully most of the writes
      // take well under 5802 us.  Some will take more, as
      // the SD library also must write to the FAT tables
      // and the SD card controller manages media erase and
      // wear leveling.  The queue1 object can buffer
      // approximately 301700 us of audio, to allow time
      // for occasional high SD card latency, as long as
      // the average write time is under 5802 us.
      // Serial.print("SD write, us=");
      // Serial.println(usec);
    }
  }
  else if (loopID == 2){
    if (_queue2->available() >= 2) {
      byte buffer[512];
      // Fetch 2 blocks from the audio library and copy
      // into a 512 byte buffer.  The Arduino SD library
      // is most efficient when full 512 byte sector size
      // writes are used.
      memcpy(buffer, _queue2->readBuffer(), 256);
      _queue2->freeBuffer();
      memcpy(buffer+256, _queue2->readBuffer(), 256);
      _queue2->freeBuffer();
      // write all 512 bytes to the SD card
      frec2.write(buffer, 512);
    }
  }
}

void AudioLooperDual::stopRecording(int loopID) {
  if (loopID == 1){
    Serial.println("stopRecording LOOP 1");
    _queue1->end();
    if (_mode1 == 1) {
      while (_queue1->available() > 0) {
        frec1.write((byte*)_queue1->readBuffer(), 256);
        _queue1->freeBuffer();
      }
      frec1.close();
    }
    _mode1 = 0;
  }
  else if (loopID == 2){
    Serial.println("stopRecording LOOP 2");
    _queue2->end();
    if (_mode2 == 1) {
      while (_queue2->available() > 0) {
        frec2.write((byte*)_queue2->readBuffer(), 256);
        _queue2->freeBuffer();
      }
      frec2.close();
    }
    _mode2 = 0;
  }
}

void AudioLooperDual::startPlaying(int loopID) {
  if (loopID == 1){
    Serial.println("startPlaying LOOP 1");
    _playSdRaw1->play("RECORD_1.RAW");
    _mode1 = 2;
  }
  else if (loopID == 2){
    Serial.println("startPlaying LOOP 2");
    _playSdRaw2->play("RECORD_2.RAW");
    _mode2 = 2;
  }
}

void AudioLooperDual::continuePlaying(int loopID) {
  if (loopID == 1){
    if (!_playSdRaw1->isPlaying()) {
      _playSdRaw1->play("RECORD_1.RAW");
      // GET LOOP SIZE AND SETUP BLINK
      _loopTempInterval1 = _playSdRaw1->lengthMillis();
    }
  }
  else if (loopID == 2){
    if (!_playSdRaw2->isPlaying()) {
      _playSdRaw2->play("RECORD_2.RAW");
      // GET LOOP SIZE AND SETUP BLINK
      _loopTempInterval2 = _playSdRaw2->lengthMillis();
    }
  }
}

void AudioLooperDual::stopPlaying(int loopID) {
  if (loopID == 1){
    Serial.println("stopPlaying LOOP 1");
    if (_mode1 == 2) _playSdRaw1->stop();
    _mode1 = 0;
  }
  else if (loopID == 2){
    Serial.println("stopPlaying LOOP 2");
    if (_mode2 == 2) _playSdRaw2->stop();
    _mode2 = 0;
  }
}

unsigned long AudioLooperDual::getLoopDuration(int loopID){
  if (loopID == 1){
    return _loopTempInterval1;
  }
  else if (loopID == 2){
    return _loopTempInterval2;
  }
  return false;
}