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
#include "DualAudioLooper.h"
#include "../GlobalInit/GlobalInit.h"


DualAudioLooper::DualAudioLooper(){
    // GUItool: begin automatically generated code
    _i2s1 = new AudioInputI2S;       
    _drum = new AudioSynthSimpleDrum;  
    _string = new AudioSynthKarplusStrong;     
    _inputAmp = new AudioAmplifier;   
    _inputMixer = new AudioMixer4; 
    _looperMixer = new AudioMixer4;
    _queue1 = new AudioRecordQueue;     
    _queue2 = new AudioRecordQueue;    
    _playSdRaw1 = new AudioPlaySdRaw; 
    _playSdRaw2 = new AudioPlaySdRaw;
    _outputMixer = new AudioMixer4;
    _i2s2 = new AudioOutputI2S;
    _audioShield = new AudioControlSGTL5000;

    _patchCord1 = new AudioConnection; 
    _patchCord2 = new AudioConnection; 
    _patchCord3 = new AudioConnection; 
    _patchCord4 = new AudioConnection; 
    _patchCord5 = new AudioConnection; 
    _patchCord6 = new AudioConnection; 
    _patchCord7 = new AudioConnection; 
    _patchCord8 = new AudioConnection; 
    _patchCord9 = new AudioConnection; 
    _patchCord10 = new AudioConnection;
    _patchCord11 = new AudioConnection; 
    _patchCord12 = new AudioConnection; 
    _patchCord13 = new AudioConnection; 
    _patchCord14 = new AudioConnection;
    // GUItool: end automatically generated code
}

void DualAudioLooper::init(){

    Serial.println("Init looper");
    // CONNECT
    _patchCord1->connect(*_i2s1, 0, *_inputAmp, 0);
    _patchCord2->connect(*_drum, 0, *_inputMixer, 2);
    _patchCord3->connect(*_string, 0, *_inputMixer, 1);
    _patchCord4->connect(*_inputAmp, 0, *_inputMixer, 0);
    _patchCord5->connect(*_inputMixer, 0, *_outputMixer, 0);
    _patchCord6->connect(*_inputMixer, 0, *_looperMixer, 0);
    _patchCord7->connect(*_looperMixer, *_queue1);
    _patchCord8->connect(*_looperMixer, *_queue2);
    _patchCord9->connect(*_playSdRaw1, 0, *_outputMixer, 1);
    _patchCord10->connect(*_playSdRaw1, 0, *_looperMixer, 1);
    _patchCord11->connect(*_playSdRaw2, 0, *_outputMixer, 2);
    _patchCord12->connect(*_playSdRaw2, 0, *_looperMixer, 2);
    _patchCord13->connect(*_outputMixer, 0, *_i2s2, 0);
    _patchCord14->connect(*_outputMixer, 0, *_i2s2, 1);

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
    _inputMixer->gain(2, 0.5);
    _inputMixer->gain(3, 0);

    // INIT LOOPER MIXER
    _looperMixer->gain(0, 1.0);
    _looperMixer->gain(1, 0);
    _looperMixer->gain(2, 0);
    _looperMixer->gain(3, 0);
    
    // INIT OUTPUT MIXER
    _outputMixer->gain(0, 0.7);
    _outputMixer->gain(1, 0.7);
    _outputMixer->gain(2, 0.0);
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
    
    // MAIN LOOP TEMPO
    continuePlaying();
    delay(200);
    stopPlaying();

    // End INIT
    Serial.println("Audio Shield initialized");
}

void DualAudioLooper::drumOn(int freq, int duration){
  _drum->frequency(freq);
  _drum->length(duration);  
  _drum->noteOn();
}

void DualAudioLooper::noteOn(int freq){
    _string->noteOn(freq, 0.8);
    _note_is_playing = true;
    _previousMillis = millis();
}

void DualAudioLooper::noteOff(){
    if (millis() - _previousMillis > _noteDurationMillis){
    _string->noteOff(0.8);
    _note_is_playing = false;
    }
}

void DualAudioLooper::set_volume(float vol){
    _audioShield->volume(vol);
    _audioShield->dacVolume(vol);
}

void DualAudioLooper::run(){
    // PLAY STRING
    if (_note_is_playing){this->noteOff();}
    
    // If LOOP 1 is playing or recording, carry on...
    if (_mode == 1) {this->continueRecording();}
    if (_mode == 2) {this->continuePlaying();}
    if (_mode == 3) {this->continueOverDubbing();}
}

//////////////////////////////////////////////
////////////// UTILITY FUNCTIONS /////////////
//////////////////////////////////////////////

int DualAudioLooper::get_mode(){
  return _mode;
}

void DualAudioLooper::setMixerToMainRecording(){
    _outputMixer->gain(0, 0.8);
    _outputMixer->gain(1, 0.0);
    _outputMixer->gain(2, 0.0);
    _outputMixer->gain(3, 0);
}

void DualAudioLooper::setMixerToOverDubbing(){
    _outputMixer->gain(0, 0.8);
    _outputMixer->gain(1, 0.8);
    _outputMixer->gain(2, 0.0);
    _outputMixer->gain(3, 0);
}

//////////////////////////////////////////////
// FUNCTION TO RECORD AND PLAY FROM SD CARD //
//////////////////////////////////////////////

void DualAudioLooper::startRecording() {
  Serial.println("startRecording MAIN LOOP");
  if (SD.exists("MAIN_LOOP.RAW")) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove("MAIN_LOOP.RAW");
  }
  frec1 = SD.open("MAIN_LOOP.RAW", FILE_WRITE);
  if (frec1) {
    _queue1->begin();
    _mode = 1;
  }
}

void DualAudioLooper::continueRecording() {
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

void DualAudioLooper::stopRecording() {
  Serial.println("stopRecording MAIN LOOP");
  _queue1->end();
  if (_mode == 1) {
    while (_queue1->available() > 0) {
      frec1.write((byte*)_queue1->readBuffer(), 256);
      _queue1->freeBuffer();
    }
    frec1.close();
  }
  _mode = 0;
}

void DualAudioLooper::startPlaying() {
  Serial.println("startPlaying MAIN LOOP");
  _playSdRaw1->play("MAIN_LOOP.RAW");
  _mode = 2;
}

void DualAudioLooper::continuePlaying() {
  if (!_playSdRaw1->isPlaying()) {
    _playSdRaw1->play("MAIN_LOOP.RAW");
    // GET LOOP SIZE AND SETUP BLINK
    _loopTempInterval = _playSdRaw1->lengthMillis();
  }
}

void DualAudioLooper::stopPlaying() {
  Serial.println("stopPlaying LOOP 1");
  if (_mode == 2) _playSdRaw1->stop();
  _mode = 0;
}

void DualAudioLooper::startOverDubbing() {
  Serial.println("start OverDub");
  if (SD.exists("SECONDARY_LOOP.RAW")) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove("SECONDARY_LOOP.RAW");
  }
  frec1 = SD.open("SECONDARY_LOOP.RAW", FILE_WRITE);
  if (frec1) {
    _queue1->begin();
    _mode = 3;
    this->setMixerToOverDubbing();
  }
}

void DualAudioLooper::continueOverDubbing() {
  this->continuePlaying();
  if (_queue1->available() >= 2) {
    byte buffer[512];
    memcpy(buffer, _queue1->readBuffer(), 256);
    _queue1->freeBuffer();
    memcpy(buffer+256, _queue1->readBuffer(), 256);
    _queue1->freeBuffer();
    frec1.write(buffer, 512);
  }
}

void DualAudioLooper::stopOverDubbing() {
  Serial.println("stopRecording OVERDUBBING");
  _queue1->end();
  if (_mode == 3) {
    while (_queue1->available() > 0) {
      frec1.write((byte*)_queue1->readBuffer(), 256);
      _queue1->freeBuffer();
    }
    frec1.close();
    // RENAME FILE TO ALWAYS PLAY NEW FILE
    this->swapFiles();
    this->setMixerToMainRecording();
  }
  _mode = 0;
}

void DualAudioLooper::swapFiles(){
  size_t n;  
  uint8_t buf[256];
  this->stopPlaying();

  // REMOVE TEMP FILE
  if (SD.exists("TEMP_LOOP.RAW")) {
    SD.remove("TEMP_LOOP.RAW");
  }
  
  // OPEN MAIN LOOP AND NEW TEMP LOOP
  frec1 = SD.open("MAIN_LOOP.RAW", FILE_READ);
  frec2 = SD.open("TEMP_LOOP.RAW", FILE_WRITE);
  while ((n = frec1.read(buf, sizeof(buf))) > 0) {
    frec2.write(buf, n);
  }
  // REMOVE MAIN LOOP
  if (SD.exists("MAIN_LOOP.RAW")) {
    SD.remove("MAIN_LOOP.RAW");
  }

  // WRITE NEW LOOP TO MAIN LOOP
  frec1 = SD.open("SECONDARY_LOOP.RAW", FILE_READ);
  frec2 = SD.open("MAIN_LOOP.RAW", FILE_WRITE);
  while ((n = frec1.read(buf, sizeof(buf))) > 0) {
    frec2.write(buf, n);
  }
  // REMOVE MAIN LOOP
  if (SD.exists("SECONDARY_LOOP.RAW")) {
    SD.remove("SECONDARY_LOOP.RAW");
  }

}

void DualAudioLooper::cancelOverDubbing() {
  size_t n;  
  uint8_t buf[256];
  this->stopPlaying();
  if (SD.exists("MAIN_LOOP.RAW")) {
    SD.remove("MAIN_LOOP.RAW");
  }
  frec1 = SD.open("TEMP_LOOP.RAW", FILE_READ);
  frec2 = SD.open("MAIN_LOOP.RAW", FILE_WRITE);
  while ((n = frec1.read(buf, sizeof(buf))) > 0) {
    frec2.write(buf, n);
  }
}

unsigned long DualAudioLooper::getLoopDuration(){
  return _loopTempInterval;
}