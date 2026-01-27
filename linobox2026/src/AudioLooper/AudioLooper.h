/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef AudioLooper_h
#define AudioLooper_h
#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "../GlobalInit/GlobalInit.h"


class AudioLooper
{
  public:
    AudioLooper();
    void init();

    // MAIN FUNCTION
    void run();

    // UTILITY FUNCTIONS
    void set_volume(float value);
    void noteOn(int freq);
    void noteOff();
    int get_mode();

    // LOOPER FUNCTIONS
    void startRecording();
    void continueRecording();
    void stopRecording();
    void startPlaying();
    void continuePlaying();
    void stopPlaying();

  private:

    // AUDIO OBJECT
    AudioInputI2S            *_i2s1;
    AudioAmplifier           *_inputAmp;
    AudioSynthKarplusStrong  *_stringSynth;
    AudioMixer4              *_inputMixer;
    AudioRecordQueue         *_queue;
    AudioPlaySdRaw           *_playSdRaw;
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

    // PARAMETERS
    bool _note_is_playing = false;
    unsigned long _previousMillis = 0;
    unsigned long _noteDurationMillis = 250;
    unsigned long _loopTempInterval = 0;
    File frec;  // The file where data is recorded
    int _mode = 0;
};

#endif
