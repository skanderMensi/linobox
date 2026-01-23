/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef PotTeensyMicOnly_h
#define PotTeensyMicOnly_h
#include "Arduino.h"
#include "../GlobalInitMicOnly/GlobalInitMicOnly.h"


class PotTeensyMicOnly
{
  public:
    PotTeensyMicOnly();
    void init(int pot_id);

    // MAIN FUNCTION
    bool run_potentiometer();

    // READ ENCODER AND PUSH BUTTON
    void read_pot();

    // UTILITY FUNCTIONS
    float pot_to_parameter(int pot_value);
    float get_parameter_value();
    int16_t get_pot_value();
    void set_parameter_range(float min_param, float max_param);

  private:

    // INITIALIZE SERIAL
    int _pot_id;
    uint8_t _pin;

    // ENCODER READ
    float _minParam = parameterMin;
    float _maxParam = parameterMax;
    int16_t _pot_value = 0;
    int16_t _previous_pot_value = 0;
    float _parameter_value = 0;
    bool _new_parameter = true;

};

#endif
