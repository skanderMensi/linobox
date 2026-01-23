/*
  ScreenTeensy.cpp - Library for teensy encoders.
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "PadTeensy.h"
#include "../GlobalInit/GlobalInit.h"
#include "Adafruit_NeoTrellis.h"


PadTeensy::PadTeensy(){
  // INIT INTERUPT PIN
  pinMode(trellisInterruptPin, INPUT);
  _trellis = new Adafruit_NeoTrellis;
}

void PadTeensy::init(TrellisCallback (*callback)(keyEvent evt)){

  // INIT PAD
  while (!_trellis->begin()) {
    Serial.println("Could not start trellis, check wiring?");
    delay(1);
  }
  Serial.println("NeoPixel Trellis started");

  // SET callback
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
   _trellis->activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
   _trellis->activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
   _trellis->registerCallback(i, callback);
  }

  // ANIMATE
  this->display_animation();

  // CLEAR SCREEN
  this->clear_pad();
  _previousMillis = millis();
}

bool PadTeensy::run(){

  // READ BUTTON PRESS
  if(!digitalRead(trellisInterruptPin)){
    _trellis->read(false);
  }

  // DISPLAY ANIMATION EVERY 10 SECONDS
  if (_mode == 0){  // ANIMATED LOGO
    this->display_animation();
  }
  else if (_mode == 1){  // WAIT 10 SEC
    if (millis() - _previousMillis > _delayMillisBeforeAnimation){
      _mode = 0;
      _previousMillis = millis();
    }
  }
  return _new_message;
}

void PadTeensy::clear_pad(){
  for (uint16_t i=0; i<_trellis->pixels.numPixels(); i++){
    _trellis->pixels.setPixelColor(i, 0x000000);
    _trellis->pixels.show();
  }
  _trellis->pixels.setPixelColor(0,0x008000);
  _trellis->pixels.setPixelColor(1,0xFF0000);
}

void PadTeensy::display_animation(){
  if (millis() - _previousMillisAnimation > _delayMillis){
    if (_animation_mode){
      _trellis->pixels.setPixelColor(_pad_id_animation, this->get_color(map(_pad_id_animation, 0, _trellis->pixels.numPixels(), 0, 255)));
    }
    _trellis->pixels.show();
    _pad_id_animation = _pad_id_animation + 1;
    if (_pad_id_animation >= _trellis->pixels.numPixels()){
      _pad_id_animation = 0;
      _animation_mode = !_animation_mode;
      if (_animation_mode){
        _mode = 1;
        this->clear_pad();
        _previousMillis = millis();
      }
    }
    _previousMillisAnimation = millis();
  }
}

uint32_t PadTeensy::get_color(byte button_position) {
  // Input a value 0 to 255 to get a color value.
  // The colors are a transition r - g - b - back to r.
  if(button_position < 85) {
    return _trellis->pixels.Color(button_position * 3, 255 - button_position * 3, 0);
  }
  else if(button_position < 170) {
    button_position -= 85;
    return _trellis->pixels.Color(255 - button_position * 3, 0, button_position * 3);
  }
  else {
    button_position -= 170;
    return _trellis->pixels.Color(0, button_position * 3, 255 - button_position * 3);
  }
  return 0;
}

TrellisCallback PadTeensy::callback_trellis(keyEvent evt){
  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){  // callback for key presses
    _trellis->pixels.setPixelColor(evt.bit.NUM, this->get_color(map(evt.bit.NUM, 0, _trellis->pixels.numPixels(), 0, 255))); //on rising
    _new_message = true;
    _message = evt.bit.NUM;
  }
  else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
    _trellis->pixels.setPixelColor(evt.bit.NUM, 0); //off falling
    _trellis->pixels.show();
  }
  _trellis->pixels.show();
  return 0;
}

void PadTeensy::reset_message(){
  _new_message = false;
}

uint8_t PadTeensy::get_message(){
  return _message;
}
