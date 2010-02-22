#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised.h"
#include "queable.h"
#include <avr/interrupt.h>

class Scheduler {
public:
  // class Task : protected Prioritised<Queable<Task> > {
  class Task : protected Prioritised<Task> {
  private:
    // typedef Prioritised<Queable<Task> > Base;
    typedef Prioritised<Task> Base;
    
    bool enqueue();
    void start();
    void dequeue();
    
  public:
    Task(unsigned int level) : Base(level) { }
    
    void operator ()() { enqueue(); }
    
    virtual void run() = 0;
  };

  void init() { sei(); }
};

#endif
