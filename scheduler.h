#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised_queable.h"
#include <avr/interrupt.h>

class Scheduler {
public:
  class Task : protected PrioritisedQueable<Task> {
  private:
    bool enqueue();
    void start();
    void dequeue();
    
  public:
    Task(unsigned int level) : PrioritisedQueable<Task>(level) { }
    
    void operator ()() { enqueue(); }
    
    virtual void run() = 0; // TODO: can we use a template to remove this virtual method too?
  };

  void init() { sei(); }
};

#endif
