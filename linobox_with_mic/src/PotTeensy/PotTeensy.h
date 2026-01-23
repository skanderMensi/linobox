/*
  PotTeensy.h - Library for teensy potentiometer.
  Created by S. Mensi, September, 2022.
*/

#ifndef PotTeensy_h
#define PotTeensy_h
#include "Arduino.h"
#include "../GlobalInit/GlobalInit.h"


class PotTeensy
{
  public:
    PotTeensy();
    void init(int pot_id);

    // MAIN FUNCTION
    bool run_potentiometer();

    // READ ENCODER AND PUSH BUTTON
    void read_pot();

    // UTILITY FUNCTIONS
    float pot_to_parameter(int pot_value);
    float get_parameter_value();
    int16_t get_pot_value();

  private:

    // INITIALIZE SERIAL
    int _pot_id;
    uint8_t _pin;

    // ENCODER READ
    int16_t _pot_value = 0;
    int16_t _previous_pot_value = 0;
    float _parameter_value = 0;
    bool _new_parameter = true;

};

#endif
