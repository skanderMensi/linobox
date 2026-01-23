#include "src/GlobalInit/GlobalInit.h"
#include "src/PadTeensy/PadTeensy.h"
#include <SPI.h>
#include <Wire.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>


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

// SETUP
void setup() {

  // INIT SERIAL
  Serial.begin(teensyBaudRate);

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
  level = 0;

  // init pot
  pot = analogRead(potPin);
  previous_pot = pot;
  pot_value = 0;

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
  if (abs(val - previous_val) >= 8){
    previous_val = val;
    if (val <= 8){
      level = 0.0;
    }
    else if (val >= 1015){
      level = 1.0;
    }
    else{
      level = float(map(val, 0, 1023, 0, 100)) / 100.0;
    }
    Serial.print("volume = ");
    Serial.println(level);
  }

  // FIRST POTENTIOMETER
  pot = analogRead(potPin);
  if (abs(pot - previous_pot) >= 8){
    previous_pot = pot;
    if (pot <= 8){
      pot_value = 0.0;
    }
    else if (pot >= 1015){
      pot_value = 1.0;
    }
    else{
      pot_value = float(map(pot, 0, 1023, 0, 100)) / 100.0;
    }
    Serial.print("potentiometer = ");
    Serial.println(pot_value);
  }

}

TrellisCallback trellis_callback_wrapper(keyEvent evt){
  return pad.callback_trellis(evt);
}
