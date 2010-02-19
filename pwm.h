#ifndef __PWM_H_
#define __PWM_H_


class Pwm {
public:  
  enum { frequency = 4000, resolution = F_CPU / frequency / 2 };
  
  Pwm();
  void init() { }
};

#endif
