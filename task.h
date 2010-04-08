#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "prioritised.h"

class Task : protected Prioritised<Task> {
private:
  typedef Prioritised<Task> Base;
  
  bool enqueue();
  void start();
  void dequeue();
  
public:
  Task(unsigned int level) : Base(level) { }
  
  void operator ()() { enqueue(); }
  virtual void run() = 0;
  bool pending() { return Base::pending(); }
};

#endif
