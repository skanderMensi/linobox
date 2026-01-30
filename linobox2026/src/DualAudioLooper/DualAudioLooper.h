/*
  DualAudioLooper.h - Library for teensy looper with 2 channels.
  Created by S. Mensi, September, 2022.
*/

#ifndef DualAudioLooper_h
#define DualAudioLooper_h
#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "../GlobalInit/GlobalInit.h"


class DualAudioLooper
{
  public:
    DualAudioLooper();
    void init();

    // MAIN FUNCTION
    void run();

    // UTILITY FUNCTIONS
    void set_volume(float value);
    void drumOn(int freq, int duration);
    void noteOn(int freq);
    void noteOff();
    int get_mode();
    void setMixerToMainRecording();
    void setMixerToOverDubbing();

    // LOOPER FUNCTIONS
    void startRecording();
    void continueRecording();
    void stopRecording();
    void startPlaying();
    void continuePlaying();
    void stopPlaying();
    void startOverDubbing();
    void continueOverDubbing();
    void stopOverDubbing();
    void cancelOverDubbing();
    void swapFiles();
    unsigned long getLoopDuration();

  private:
  
    // AUDIO OBJECT
    AudioInputI2S            *_i2s1;       
    AudioSynthSimpleDrum     *_drum;       
    AudioSynthKarplusStrong  *_string;     
    AudioAmplifier           *_inputAmp;   
    AudioMixer4              *_inputMixer; 
    AudioMixer4              *_looperMixer;
    AudioRecordQueue         *_queue1;     
    AudioRecordQueue         *_queue2;     
    AudioPlaySdRaw           *_playSdRaw1; 
    AudioPlaySdRaw           *_playSdRaw2; 
    AudioMixer4              *_outputMixer;
    AudioOutputI2S           *_i2s2;
    AudioControlSGTL5000     *_audioShield;

    // CONNECTION
    AudioConnection         *_patchCord1;
    AudioConnection         *_patchCord2;
    AudioConnection         *_patchCord3;
    AudioConnection         *_patchCord4;
    AudioConnection         *_patchCord5;
    AudioConnection         *_patchCord6;
    AudioConnection         *_patchCord7;
    AudioConnection         *_patchCord8;
    AudioConnection         *_patchCord9;
    AudioConnection         *_patchCord10;
    AudioConnection         *_patchCord11;
    AudioConnection         *_patchCord12;
    AudioConnection         *_patchCord13;
    AudioConnection         *_patchCord14;

    // PARAMETERS
    bool _note_is_playing = false;
    unsigned long _previousMillis = 0;
    unsigned long _noteDurationMillis = 250;
    
    // LOOP
    unsigned long _loopTempInterval = 0;
    File frec1;  // The file where Loop 1 is recorded
    File frec2;  // The file where Loop 2 is recorded
    int _mode = 0;

};

#endif
