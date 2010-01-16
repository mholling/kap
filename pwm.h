#ifndef __PWM_H_
#define __PWM_H_

#include <avr/io.h>
#include "scheduler.h"

class Pwm {
private:
  enum { frequency = 4000, resolution = F_CPU / frequency / 2 };
  
public:
  Pwm();
  void init();
  inline void set_A(float a);
  inline void set_B(float b);
};

#endif
