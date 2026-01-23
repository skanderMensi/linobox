/*
  PotTeensy.cpp - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "PotTeensyMicOnly.h"
#include "../GlobalInitMicOnly/GlobalInitMicOnly.h"


PotTeensyMicOnly::PotTeensyMicOnly(){
}

void PotTeensyMicOnly::init(int pot_id){
  _pot_id = pot_id;
  _pin = potentiometerPins[_pot_id];

  // INIT ENCODER
  pinMode(_pin, INPUT);
  _pot_value = analogRead(_pin);
  _previous_pot_value = _pot_value;
  _parameter_value = pot_to_parameter(_pot_value);
}


void PotTeensyMicOnly::read_pot(){

  // READ ENCODER
  _pot_value = analogRead(_pin);

  // IF GET NEW VALUE -> UPDATE AND SEND
  if (abs(_pot_value - _previous_pot_value) > potThreshold) {
    _previous_pot_value = _pot_value;
    if (_pot_value >= potMax - potThreshold){
      _pot_value = potMax;
    }
    else if (_pot_value <= potMin + potThreshold){
      _pot_value = potMin;
    }
    _parameter_value = pot_to_parameter(_pot_value);
    _new_parameter = true;
  }
}

float PotTeensyMicOnly::pot_to_parameter(int pot_value){
  return map_int_to_float(pot_value, potMin, potMax, _minParam, _maxParam);
}

float PotTeensyMicOnly::get_parameter_value(){
  return _parameter_value;
}

int16_t PotTeensyMicOnly::get_pot_value(){
  return _pot_value;
}

void PotTeensyMicOnly::set_parameter_range(float min_param, float max_param){
  _minParam = min_param;
  _maxParam = max_param;
  this->pot_to_parameter(this->get_pot_value());
}

bool PotTeensyMicOnly::run_potentiometer(){
  bool temp_new_parameter;
  temp_new_parameter = false;
  this->read_pot();
  if (_new_parameter == true){
    temp_new_parameter = true;
    _new_parameter = false;
  }
  return temp_new_parameter;
}
