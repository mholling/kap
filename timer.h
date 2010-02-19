#ifndef __TIMER_H_
#define __TIMER_H_

#include "scheduler.h"
#include "diagnostic_task.h"

class Timer {
private:
  unsigned long int count;
  
  class Task : public Scheduler::Task {
  private:
    unsigned int count;
    
     DiagnosticTask diagnostic;
    
  public:
    Task() : Scheduler::Task(0), count(0) { }
    void run();
  };
  
   Task task;
  
public:
  enum { frequency = 35 };
  
  Timer();
  inline void init() { }
  
  void interrupt();
  
  class Stamp {
  private:
    unsigned long int value;
    
  public:
    Stamp();
    void operator ()();
    long int seconds() const;
    long int seconds_ago() const;
    bool since(long int duration_in_seconds) const;

    inline bool operator >(const Stamp& rhs) const { return value > rhs.value; }
    inline bool operator <(const Stamp& rhs) const { return value < rhs.value; }
  };

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
