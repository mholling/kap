#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"
#include "safe.h"
#include <avr/interrupt.h>

class Scheduler {
public:
  class Task : protected PrioritisedQueable<Task> {
  private:
    void start();
    void dequeue();
    
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    void operator ()();
    virtual void run() volatile = 0;
    
    inline void wait() { do { } while (next != 0); }
  };

  inline void init() volatile { sei(); }
};

#endif
