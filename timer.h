#ifndef __TIMER_H_
#define __TIMER_H_

#include "scheduler.h"
#include "safe.h"
#include "diagnostic_task.h"

class Timer {
private:
  unsigned long int count;
  
  class Task : public Scheduler::Task {
  private:
    unsigned int count;
    
    volatile DiagnosticTask diagnostic;
    
  public:
    Task() : Scheduler::Task(0), count(0) { }
    void run();
  };
  
  volatile Task task;
  
public:
  enum { frequency = 35 };
  
  Timer();
  inline void init() volatile { }
  
  void interrupt();
  
  unsigned long int timestamp() volatile { return Safe<Timer>(this)->timestamp(); }
  unsigned long int timestamp();
  
  class Diagnostic {
  private:
    const unsigned long int start;
    const char *message;
  public:
    Diagnostic(const char *message);
    ~Diagnostic();
  };
};

#endif
