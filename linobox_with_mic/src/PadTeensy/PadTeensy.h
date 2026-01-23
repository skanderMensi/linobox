/*
  PadTeensy.h - Library for teensy NeoTrellis rgb pad array.
  Created by S. Mensi, September, 2022.
*/

#ifndef PadTeensy_h
#define PadTeensy_h
#include "Arduino.h"
#include "../GlobalInit/GlobalInit.h"
#include "Adafruit_NeoTrellis.h"


class PadTeensy
{

  public:
    PadTeensy();
    void init(TrellisCallback (*callback)(keyEvent evt));

    // MAIN FUNCTION
    bool run();

    // CALLBACK FUNCTION
    TrellisCallback callback_trellis(keyEvent evt);
    void reset_message();
    uint8_t get_message();


    // UTILITY FUNCTIONS
    uint32_t get_color(byte button_position);
    void set_color(uint8_t button_position, uint32_t color);
    void display_animation();
    void clear_pad();

  private:

    // INITIALIZE SCREEN
    Adafruit_NeoTrellis *_trellis;

    // MESSAGE
    bool _new_message = false;
    uint8_t _message;

    // SCRREN UPDATE PARAMETERS
    unsigned long _delayMillisBeforeAnimation = 10000;
    unsigned long _previousMillis = 0;
    unsigned long _delayMillis = 25;
    unsigned long _previousMillisAnimation = 0;
    int _pad_id_animation = 0;
    bool _animation_mode = true;
    int _mode = 0;

};

#endif
