/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef AudioLooperDual_h
#define AudioLooperDual_h
#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "../GlobalInit/GlobalInit.h"


class AudioLooperDual
{
  public:
    AudioLooperDual();
    void init();

    // MAIN FUNCTION
    void run();

    // UTILITY FUNCTIONS
    void set_volume(float value);
    void noteOn(int freq);
    void noteOff();
    int get_mode(int loopID);

    // LOOPER FUNCTIONS
    void startRecording(int loopID);
    void continueRecording(int loopID);
    void stopRecording(int loopID);
    void startPlaying(int loopID);
    void continuePlaying(int loopID);
    void stopPlaying(int loopID);
    unsigned long getLoopDuration(int loopID);

  private:

    // AUDIO OBJECT
    AudioInputI2S            *_i2s1;
    AudioAmplifier           *_inputAmp;
    AudioSynthKarplusStrong  *_stringSynth;
    AudioMixer4              *_inputMixer;
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

    // PARAMETERS
    bool _note_is_playing = false;
    unsigned long _previousMillis = 0;
    unsigned long _noteDurationMillis = 250;
    
    // LOOP 1
    unsigned long _loopTempInterval1 = 0;
    File frec1;  // The file where data is recorded
    int _mode1 = 0;

    // LOOP 2
    unsigned long _loopTempInterval2 = 0;
    File frec2;  // The file where data is recorded
    int _mode2 = 0;
};

#endif
