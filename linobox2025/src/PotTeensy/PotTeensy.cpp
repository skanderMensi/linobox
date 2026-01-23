/*
  PotTeensy.cpp - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "PotTeensy.h"
#include "../GlobalInit/GlobalInit.h"


PotTeensy::PotTeensy(){
  // ENCODER PARAMETERS
  _parameter_min = parameterMin;
  _parameter_max = parameterMax;
}

void PotTeensy::init(int pot_id){
  _pot_id = pot_id;
  _pin = potentiometerPins[_pot_id];

  // INIT ENCODER
  pinMode(_pin, INPUT);
  _pot_value = analogRead(_pin);
  _previous_pot_value = _pot_value;
  _parameter_value = pot_to_parameter(_pot_value);
}


void PotTeensy::read_pot(){

  // READ ENCODER
  _pot_value = analogRead(_pin);

  // IF GET NEW VALUE -> UPDATE AND SEND
  if (abs(_pot_value - _previous_pot_value) > potThreshold) {
    _previous_pot_value = _pot_value;
    _parameter_value = pot_to_parameter(_pot_value);
    _new_parameter = true;
  }
}

uint16_t PotTeensy::pot_to_parameter(int pot_value){
  if (_pot_id == 2){
    return uint16_t(map(pot_value, potMin, potMax, parameterMin, parameterMax));
  }
  else{
    return uint16_t(map(pot_value, potMin, potMax, parameterMax, parameterMin));
  }
}

uint16_t PotTeensy::get_parameter_value(){
  return _parameter_value;
}

int16_t PotTeensy::get_pot_value(){
  return _pot_value;
}

bool PotTeensy::run_potentiometer(){
  bool temp_new_parameter;
  temp_new_parameter = false;
  this->read_pot();
  if (_new_parameter == true){
    temp_new_parameter = true;
    _new_parameter = false;
  }
  return temp_new_parameter;
}
