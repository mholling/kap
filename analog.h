#ifndef __ADC_H_
#define __ADC_H_

#include "resource.h"
#include <avr/io.h>
#include "scheduler.h"

class Analog : private Resource {  
public:
  unsigned int data[4];
  Analog(App* app);
  void init();
  void interrupt();
  void start_conversions();
};

#endif
