#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "pwm.h"
#include <avr/io.h>

class Motors {
  enum { standby_shift_register_bit = 1, yaw_polarity = +1, pitch_polarity = +1 };
  
public:
  Motors();
  void init();
  
  void disable();
  void enable();
  
  void yaw(float value);
  void pitch(float value);
};

#endif
