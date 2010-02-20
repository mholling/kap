#ifndef __DIAGNOSTIC_H_
#define __DIAGNOSTIC_H_

#include "timer.h"

class Diagnostic : public Timer::Task {
private:
  unsigned int count;
  
public:
  Diagnostic() : count(0) {}
  void run();
};

#endif
