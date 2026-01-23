/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef AudioEffect_h
#define AudioEffect_h
#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "../GlobalInit/GlobalInit.h"


class AudioEffect
{
  public:
    AudioEffect();
    void init();

    // MAIN FUNCTION
    void run();

    // UTILITY FUNCTIONS
    void set_frequency(int freq);
    void set_volume(float value);
    void update_input_gain(float value);
    void update_dry_wat(float value);

  private:

    // AUDIO OBJECT
    AudioInputI2S            *_i2s1;
    AudioAmplifier           *_inputAmp;
    AudioRecordQueue         *_queue;
    AudioPlaySdRaw           *_playSdRaw;
    AudioMixer4              *_inputMixer;
    AudioOutputI2S           *_i2s2;
    AudioControlSGTL5000     *_audioShield;

    AudioSynthWaveformSine   *_sine;


    // CONNECTION
    AudioConnection         *_patchCord1;
    AudioConnection         *_patchCord2;
    AudioConnection         *_patchCord3;
    AudioConnection         *_patchCord4;
    AudioConnection         *_patchCord5;
    AudioConnection         *_patchCord6;

    AudioConnection         *_patchCord7;
};

#endif
