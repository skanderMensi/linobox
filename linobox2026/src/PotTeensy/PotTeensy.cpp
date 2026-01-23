/*
  PotTeensy.cpp - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#include "Arduino.h"
#include "ResponsiveAnalogRead.h"
#include "PotTeensy.h"
#include "../GlobalInit/GlobalInit.h"


PotTeensy::PotTeensy(){}

void PotTeensy::init(int pot_id){
  _pot_id = pot_id;
  _pin = potentiometerPins[_pot_id];

  // INIT POTENTIOMETER
  _pot = new ResponsiveAnalogRead(_pin, true);
  _pot->update();
  _pot_value = _pot->getValue();
  _parameter_value = pot_to_parameter(_pot_value);
  Serial.print("Init potentiometers on pin ");
  Serial.print(_pin);
  Serial.print(" | pot value = ");
  Serial.print(_pot_value);
  Serial.print(" | parameter value = ");
  Serial.println(_parameter_value);
}


void PotTeensy::read_pot(){

  // READ POTENTIOMETER
  _pot->update();

  // IF GET NEW VALUE -> UPDATE AND SEND
  if(_pot->hasChanged()) {
    _pot_value = _pot->getValue();
    _parameter_value = pot_to_parameter(_pot_value);
    _new_parameter = true;
  }

}

float PotTeensy::pot_to_parameter(int pot_value){
  return float(map(pot_value, potMin, potMax, 0, 100)) / 100.0;
}

float PotTeensy::get_parameter(){
  return _parameter_value;
}

int16_t PotTeensy::get_pot_value(){
  return _pot_value;
}

bool PotTeensy::run(){
  bool temp_new_param;
  temp_new_param = false;
  this->read_pot();
  if (_new_parameter){
    _new_parameter = false;
    temp_new_param = true;
  }
  return temp_new_param;
}
