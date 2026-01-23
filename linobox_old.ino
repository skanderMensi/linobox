#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>

// AUDIO
AudioInputI2S            i2s_in;           //xy=68,1512
AudioOutputI2S           i2s_out;           //xy=516,149
AudioControlSGTL5000     sgtl5000_1;     //xy=141,258
AudioSynthWaveformSine   sine1;          //xy=214,1655
AudioAmplifier           amp1;           //xy=335,76
AudioConnection          patchCord1(sine1, amp1);
AudioConnection          patchCord2(amp1, 0, i2s_out, 0);
AudioConnection          patchCord3(amp1, 0, i2s_out, 1);

// TRELLIS PAD
TrellisCallback trellis_callback_wrapper(keyEvent evt);
PadTeensy pad;

int val;
int previous_val;
float level;

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

  // Init Audio
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  sine1.amplitude(0.5);
  sine1.frequency(440);

  // INIT RANDOM SEED WITH POSITION OF THE POTENTIOMETER
  int random_seed = analogRead(volPin);
  randomSeed(random_seed);
  Serial.print("Random seed set to: ");
  Serial.println(random_seed);

  // init trellis pad
  pad.init(trellis_callback_wrapper);

  val = analogRead(volPin);
  previous_val = val;
  level = float(val) / 1023.0;
  amp1.gain(level);
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

  val = analogRead(volPin);
  if (abs(val - previous_val) > 5){
    previous_val = val;
    
    level = float(val) / 1023.0;
    Serial.println(level);
    amp1.gain(level);
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}
