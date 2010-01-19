#ifndef __PWM_H_
#define __PWM_H_

#include "resource.h"

class Pwm : private Resource {
public:  
  enum { frequency = 4000, resolution = F_CPU / frequency / 2 };
  
  Pwm(App* app);
  void init();
};

#endif
