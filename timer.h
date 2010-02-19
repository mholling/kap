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
  
  class Stamp {
  private:
    unsigned long int value;
    
  public:
    Stamp();
    void operator ()() volatile;
    long int seconds() const volatile;
    long int seconds_ago() const volatile;
    bool since(long int duration_in_seconds) const volatile;

    inline bool operator >(const Stamp& rhs) const volatile { return value > rhs.value; }
    inline bool operator <(const Stamp& rhs) const volatile { return value < rhs.value; }
  };

  unsigned long int stamp() volatile { return Safe<Timer>(this)->stamp(); }
  unsigned long int stamp();
  
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
