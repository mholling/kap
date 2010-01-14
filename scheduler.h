#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"

class Scheduler {
public:
  class Task : public PrioritisedQueable<Task> {
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    virtual void operator()() = 0;
  };

private:
  volatile unsigned int level;
  void run_at_higher_level();
  
public:
  Scheduler() : level(0) { }
  void init();
  void signal(Task& task);
  void run();
};

#endif
