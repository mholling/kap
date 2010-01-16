#ifndef __TIMER_H_
#define __TIMER_H_

#include "scheduler.h"

class Timer {
private:
  enum { tick_frequency = 50 };
public:
  Timer();
  void init();
  void interrupt();
};

#endif
