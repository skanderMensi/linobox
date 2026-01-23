#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>

// AUDIO
AudioInputI2S            audioInput;
AudioSynthWaveform      sdWavPlayer;
//AudioPlaySdWav           sdWavPlayer;
AudioMixer4              mixerL;
AudioMixer4              mixerR;
AudioEffectMidSide       midsideFFT;
AudioAnalyzeFFT256       fft256;
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(audioInput, 0, mixerL, 0);
AudioConnection          patchCord2(audioInput, 0, mixerR, 0);
AudioConnection          patchCord3(sdWavPlayer, 0, mixerL, 1);
AudioConnection          patchCord4(sdWavPlayer, 0, mixerR, 1);
//AudioConnection          patchCord4(sdWavPlayer, 1, mixerR, 1);
AudioConnection          patchCord5(mixerL, 0, audioOutput, 0);
AudioConnection          patchCord6(mixerL, 0, midsideFFT, 0);
AudioConnection          patchCord7(mixerR, 0, audioOutput, 1);
AudioConnection          patchCord8(mixerR, 0, midsideFFT, 1);
AudioConnection          patchCord9(midsideFFT, 0, fft256, 0);
AudioControlSGTL5000     audioShield;

// TRELLIS PAD
TrellisCallback trellis_callback_wrapper(keyEvent evt);
PadTeensy pad;

// GLOBAL VARIABLE
int val;
int previous_val;
float level;

int pot;
int previous_pot;
float pot_value;

unsigned long previousMillis;
const int nbr_band = 4;
int freq[nbr_band] = {100, 600, 3000, 10000};
int freq_index = 0;
uint8_t fft_value[nbr_band] = {0, 0, 0, 0};

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

  // Init Audio
  AudioMemory(15);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.audioPostProcessorEnable();
  audioShield.eqSelect(2);
  audioShield.volume(0.6);

  // FFT
  midsideFFT.encode();
  fft256.windowFunction(AudioWindowHanning256);
  fft256.averageTogether(32);

  // INIT RANDOM SEED WITH POSITION OF THE POTENTIOMETER
  int random_seed = analogRead(volPin);
  randomSeed(random_seed);
  Serial.print("Random seed set to: ");
  Serial.println(random_seed);

  // init trellis pad
  pad.init(trellis_callback_wrapper);

  // init volume
  val = analogRead(volPin);
  previous_val = val;
  level = float(val) / 1023.0;
  mixerL.gain(0, 0);
  mixerR.gain(0, 0);
  change_level(level);

  // init pot
  pot = analogRead(potPin);
  previous_pot = pot;
  pot_value = float(pot) / 1023.0;

  // start wwaveform
  sdWavPlayer.begin(1.0, freq[freq_index], WAVEFORM_SINE);
  previousMillis = millis();
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

  // VOLUME POTENTIOMETER
  val = analogRead(volPin);
  if (abs(val - previous_val) > 5){
    previous_val = val;
    level = float(val) / 1023.0;
    if (val <= 5){
      level = 0;
    }
    Serial.print("volume = ");
    Serial.println(level);
    change_level(level);
  }

  // FIRST POTENTIOMETER
//  pot = analogRead(potPin);
//  if (abs(pot - previous_pot) > 5){
 //   previous_pot = pot;
 //   pot_value = float(pot) / 1023.0;
  //  if (pot <= 5){
  //    pot_value = 0;
  //  }
  //  Serial.print("potentiometer = ");
  //  Serial.println(pot_value);
  //}

  // FFT
  if (millis() - previousMillis > 5000){
    freq_index++;
    if (freq_index == 4){
      freq_index = 0;
    }
    sdWavPlayer.frequency(freq[freq_index]);
    previousMillis = millis();
  }

  if (fft256.available()) {
    Serial.print("FFT: ");
    fft_value[0] = int(map(fft256.read(0, 1), 0, 1, 0, 4)); // LOW 0 - 344 Hz
    fft_value[1] = int(map(fft256.read(2, 6), 0, 1, 0, 4)); // LOW-MID 344 - 1032 Hz
    fft_value[2] = int(map(fft256.read(7, 32), 0, 1, 0, 4)); // HIGH-MID 1032 - 5504 Hz
    fft_value[3] = int(map(fft256.read(33, 116), 0, 1, 0, 4)); // HIGH 2204 - 19952 Hz
    print_fft(fft_value, nbr_band);
    pad.display_fft(fft_value, nbr_band);
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}

void change_level(float new_level){
  mixerL.gain(1, new_level);
  mixerR.gain(1, new_level);
  audioShield.volume(new_level);
}

void print_fft(uint8_t band_array[], uint8_t nbr_band){
  for (int i=0;i<nbr_band;i++){
    Serial.print(band_array[i]);
    Serial.print(" ");
  }
  Serial.println();
}
