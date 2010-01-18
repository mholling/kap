#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"

class Scheduler {
public:
  class Task : public PrioritisedQueable<Task> {
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    virtual void operator()() = 0;
    inline void complete() { dequeue(); }
  };

private:
  void run_tasks(Task* new_task = 0);
  void idle();
  volatile Task* current_task;
  
public:
  Scheduler() : current_task(0) { }
  void init();
  void signal(Task& task);
  void run();
};

#endif
