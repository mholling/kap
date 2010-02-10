#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"

class Scheduler {
public:
  class Task : protected PrioritisedQueable<Task> {
  private:
    void start();
    
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    void operator ()();
    virtual void run() = 0;
    
    inline bool pending() { return PrioritisedQueable<Task>::pending(); }
    inline static Task& head() { return PrioritisedQueable<Task>::head(); }
  };

  inline void init() { sei(); }
};

#endif
