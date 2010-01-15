#ifndef __TIMER_H_
#define __TIMER_H_

#include "scheduler.h"

class Timer {
private:
  int count;
public:
  Timer();
  void init();
  void interrupt();
  
  class Task : public Scheduler::Task {
  public:
    Task() : Scheduler::Task(5) { }
    void operator ()();
  } task;
};

#endif
