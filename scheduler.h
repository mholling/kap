#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"

class Scheduler {
public:
  class Task : protected PrioritisedQueable<Task> {
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    virtual void operator()() = 0;
    inline void complete() { dequeue(); }

    inline void enqueue() { return PrioritisedQueable<Task>::enqueue(); }
    inline bool pending() { return PrioritisedQueable<Task>::pending(); }
    inline static Task& head() { return PrioritisedQueable<Task>::head(); }
    inline static bool any() { return PrioritisedQueable<Task>::any(); }
  };

private:
  void run_tasks(Task* new_task = 0);
  volatile Task* current_task;
  
public:
  Scheduler() : current_task(0) { }
  inline void init() { sei(); }
  void signal(Task& task);
  void run();
};

#endif
