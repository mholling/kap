#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "resource.h"
#include "prioritised_queable.h"

class Scheduler : private Resource {
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
  Scheduler(App* app) : Resource(app), current_task(0) { }
  void init();
  void signal(Task& task);
  void run();
};

#endif
