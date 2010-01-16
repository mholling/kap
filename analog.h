#ifndef __ADC_H_
#define __ADC_H_

#include <avr/io.h>
#include "scheduler.h"

class Analog {  
public:
  unsigned int data[4];
  Analog();
  void init();
  inline void interrupt();
  void start_conversions();
  
  class ShowResults : public Scheduler::Task {
  public:
    ShowResults() : Scheduler::Task(5) { }
    void operator()();
  } show_results;
};

#endif
