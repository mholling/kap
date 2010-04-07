#ifndef __TIMER_H_
#define __TIMER_H_

#include "scheduler.h"

class Timer {
private:
  unsigned long int count;
  
  class TimedTasks : public Scheduler::Task {
  public:
    TimedTasks() : Scheduler::Task(Timer::Task::priority + 1) { }
    void run();
  } timed_tasks;
  
public:
  enum { frequency = 35, ocr2a = F_CPU / 1024 / frequency - 1 };
  static inline const float dt() { return static_cast<float>(ocr2a + 1) * 1024 / F_CPU; }
  
  Timer();
  void init() { }
  
  void interrupt();
  
  class Task : public Scheduler::Task {
  public:
    enum { priority = 100 };
    Task() : Scheduler::Task(priority) { }
  };
  
  class Stamp {
  private:
    unsigned long int value;
    
  public:
    Stamp();
    void operator ()();
    long int seconds() const;
    long int seconds_ago() const;
    bool since(long int duration_in_seconds) const;

    bool operator >(const Stamp& rhs) const { return value > rhs.value; }
    bool operator <(const Stamp& rhs) const { return value < rhs.value; }
  };

  unsigned long int stamp();
  
  class Diagnostic {
  private:
    const unsigned long int start;
    const char *message;
    const bool report;
  public:
    Diagnostic(const char *message, int seconds = 0);
    ~Diagnostic();
  };
};

#endif
