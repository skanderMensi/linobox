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
    uint16_t get_message();


    // UTILITY FUNCTIONS
    uint32_t get_color(byte button_position);
    void display_animation();
    void init_pad();
    void black_all();
    void blink_led();
    void set_loop_duration(int loopID, unsigned long duration);
    void blink_loop_status(int loopID, bool status);
    void blink_loop();

  private:

    // INITIALIZE SCREEN
    Adafruit_NeoTrellis *_trellis;

    // MESSAGE
    bool _new_message = false;
    uint16_t _message;

    // LED COLOR
    uint32_t led_colors[nbrLed];
    bool blink_status[nbrLed];
    bool led_status[nbrLed] = {true, false, false, false,
                              true, false, false, false,
                              true, false, false, false,
                              true, false, false, false};
    bool blink_state = false;

    // SCRREN UPDATE PARAMETERS
    unsigned long _delayMillisBeforeAnimation = 10000;
    unsigned long _previousMillis = 0;
    unsigned long _delayMillis = 25;
    unsigned long _previousMillisAnimation = 0;
    unsigned long _delayMillisBlink = 1000;
    unsigned long _previousMillisBlink = 0;
    int _pad_id_animation = 0;
    bool _animation_mode = true;
    int _mode = 1;

    // LOOP 1
    unsigned long _loopDuration1 = 100;
    elapsedMillis _sinceLastLoop1 = 0;
    elapsedMillis _sinceLastLoopON1 = 0;
    bool _blink_loop_status1 = false;
    
    // LOOP 2
    unsigned long _loopDuration2 = 100;
    elapsedMillis _sinceLastLoop2 = 0;
    elapsedMillis _sinceLastLoopON2 = 0;
    bool _blink_loop_status2 = false;

};

#endif
