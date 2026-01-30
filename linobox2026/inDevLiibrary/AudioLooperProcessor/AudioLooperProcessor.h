/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef AudioLooperProcessor_h
#define AudioLooperProcessor_h
#include "Arduino.h"
#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "../GlobalInit/GlobalInit.h"


class AudioLooperProcessor
{
  public:
    AudioLooperProcessor();
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
    AudioMixer4              *_inputEffectMixer;
    AudioAmplifier           *_inputAmp;
    AudioSynthSimpleDrum     *_drum;
    AudioMixer4              *_looperMixer;
    AudioMixer4              *_inputMixer;
    AudioAmplifier           *_ampBandPassReverb;
    AudioAmplifier           *_ampFlanger;
    AudioAmplifier           *_ampLowPass;
    AudioAmplifier           *_ampDelay;
    AudioAmplifier           *_ampBitCrusher;
    AudioAmplifier           *_ampGranular;
    AudioRecordQueue         *_queue1;
    AudioAmplifier           *_ampReverb;
    AudioRecordQueue         *_queue2;
    AudioEffectFlange        *_flanger;
    AudioEffectReverb        *_reverb;
    AudioPlaySdRaw           *_playSdRaw2;
    AudioPlaySdRaw           *_playSdRaw1;
    AudioEffectDelay         *_delay;
    AudioFilterBiquad        *_bandPass;
    AudioEffectBitcrusher    *_bitCursher;
    AudioEffectGranular      *_granular;
    AudioFilterLadder        *_lowPass;
    AudioEffectReverb        *_reverbBandPass;
    AudioMixer4              *_outputEffectMixer1;
    AudioMixer4              *_outputMixer;
    AudioMixer4              *_outputEffectMixer2;
    AudioMixer4              *_outputEffectMixer3;
    AudioMixer4              *_finalMixer;
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
    AudioConnection         *_patchCord15;
    AudioConnection         *_patchCord16;
    AudioConnection         *_patchCord17;
    AudioConnection         *_patchCord18;
    AudioConnection         *_patchCord19;
    AudioConnection         *_patchCord20;
    AudioConnection         *_patchCord21;
    AudioConnection         *_patchCord22;
    AudioConnection         *_patchCord23;
    AudioConnection         *_patchCord24;
    AudioConnection         *_patchCord25;
    AudioConnection         *_patchCord26;
    AudioConnection         *_patchCord27;
    AudioConnection         *_patchCord28;
    AudioConnection         *_patchCord29;
    AudioConnection         *_patchCord30;
    AudioConnection         *_patchCord31;
    AudioConnection         *_patchCord32;
    AudioConnection         *_patchCord33;
    AudioConnection         *_patchCord34;
    AudioConnection         *_patchCord35;
    AudioConnection         *_patchCord36;
    AudioConnection         *_patchCord37;
    AudioConnection         *_patchCord38;
    AudioConnection         *_patchCord39;
    AudioConnection         *_patchCord40;

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
