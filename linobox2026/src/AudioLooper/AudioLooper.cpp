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
#include "AudioLooper.h"
#include "../GlobalInit/GlobalInit.h"


AudioLooper::AudioLooper(){
    // GUItool: begin automatically generated code
    _i2s1 = new AudioInputI2S;
    _inputAmp = new AudioAmplifier;
    _stringSynth = new AudioSynthKarplusStrong;
    _inputMixer = new AudioMixer4;
    _queue = new AudioRecordQueue;
    _playSdRaw = new AudioPlaySdRaw;
    _outputMixer = new AudioMixer4;
    _i2s2 = new AudioOutputI2S;

    _patchCord1 = new AudioConnection;  //(i2s1, 0, _inputAmp, 0);
    _patchCord2 = new AudioConnection;  //(_inputAmp, 0, _inputMixer, 0);
    _patchCord3 = new AudioConnection;  //(_stringSynth, 0, _inputMixer, 1);
    _patchCord4 = new AudioConnection;  //(_inputMixer, _queue);
    _patchCord5 = new AudioConnection;  //(_inputMixer, 0, _outputMixer, 0);
    _patchCord6 = new AudioConnection;  //(_playSdRaw, 0, _outputMixer, 1);
    _patchCord7 = new AudioConnection;  //(_outputMixer, 0, _i2s2, 0);
    _patchCord8 = new AudioConnection;  //(_outputMixer, 0, _i2s2, 1);
    
    _audioShield = new AudioControlSGTL5000;
    // GUItool: end automatically generated code
}

void AudioLooper::init(){

    Serial.println("Init looper");
    // CONNECT
    _patchCord1->connect(*_i2s1, 0, *_inputAmp, 0); 
    _patchCord2->connect(*_inputAmp, 0, *_inputMixer, 0);
    _patchCord3->connect(*_stringSynth, 0, *_inputMixer, 1);
    _patchCord4->connect(*_inputMixer, *_queue);
    _patchCord5->connect(*_inputMixer, 0, *_outputMixer, 0);
    _patchCord6->connect(*_playSdRaw, 0, *_outputMixer, 1);
    _patchCord7->connect(*_outputMixer, 0, *_i2s2, 0);
    _patchCord8->connect(*_outputMixer, 0, *_i2s2, 1);

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
    _outputMixer->gain(2, 0);
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
    
    // LOOP TEMPO
    continuePlaying();
    delay(200);
    stopPlaying();

    // End INIT
    Serial.println("Audio Shield initialized");
}


void AudioLooper::noteOn(int freq){
    _stringSynth->noteOn(freq, 0.8);
    _note_is_playing = true;
    _previousMillis = millis();
}

void AudioLooper::noteOff(){
    if (millis() - _previousMillis > _noteDurationMillis){
    _stringSynth->noteOff(0.8);
    _note_is_playing = false;
    }
}

void AudioLooper::set_volume(float vol){
    _audioShield->volume(vol);
    _audioShield->dacVolume(vol);
}

void AudioLooper::run(){
    if (_note_is_playing){
        this->noteOff();
    }
    
    // If we're playing or recording, carry on...
    if (_mode == 1) {this->continueRecording();}
    if (_mode == 2) {this->continuePlaying();}

}

//////////////////////////////////////////////
////////////// UTILITY FUNCTIONS /////////////
//////////////////////////////////////////////

int AudioLooper::get_mode(){
    return _mode;
}

//////////////////////////////////////////////
// FUNCTION TO RECORD AND PLAY FROM SD CARD //
//////////////////////////////////////////////

void AudioLooper::startRecording() {
  Serial.println("startRecording");
  if (SD.exists("RECORD.RAW")) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove("RECORD.RAW");
  }
  frec = SD.open("RECORD.RAW", FILE_WRITE);
  if (frec) {
    _queue->begin();
    _mode = 1;
  }
}

void AudioLooper::continueRecording() {
  if (_queue->available() >= 2) {
    byte buffer[512];
    // Fetch 2 blocks from the audio library and copy
    // into a 512 byte buffer.  The Arduino SD library
    // is most efficient when full 512 byte sector size
    // writes are used.
    memcpy(buffer, _queue->readBuffer(), 256);
    _queue->freeBuffer();
    memcpy(buffer+256, _queue->readBuffer(), 256);
    _queue->freeBuffer();
    // write all 512 bytes to the SD card
    // elapsedMicros usec = 0;
    frec.write(buffer, 512);
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

void AudioLooper::stopRecording() {
  Serial.println("stopRecording");
  _queue->end();
  if (_mode == 1) {
    while (_queue->available() > 0) {
      frec.write((byte*)_queue->readBuffer(), 256);
      _queue->freeBuffer();
    }
    frec.close();
  }
  _mode = 0;
}

void AudioLooper::startPlaying() {
  Serial.println("startPlaying");
  _playSdRaw->play("RECORD.RAW");
  _mode = 2;
}

void AudioLooper::continuePlaying() {
  if (!_playSdRaw->isPlaying()) {
    _playSdRaw->play("RECORD.RAW");
    // GET LOOP SIZE AND SETUP BLINK
    _loopTempInterval = _playSdRaw->lengthMillis();
  }
}

void AudioLooper::stopPlaying() {
  Serial.println("stopPlaying");
  if (_mode == 2) _playSdRaw->stop();
  _mode = 0;
}

unsigned long AudioLooper::getLoopDuration(){
    return _loopTempInterval;
}