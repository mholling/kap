#ifndef __TIMER_H_
#define __TIMER_H_

#include "resource.h"
#include "scheduler.h"

class Timer : private Resource {
private:
  enum { frequency = 50 };
public:
  Timer(App* app);
  void init();
  void interrupt();
};

#endif
