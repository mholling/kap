#ifndef __TIMER_H_
#define __TIMER_H_

#include "task.h"

class Timer {
private:
  unsigned long int count;
  
  class TimedTasks : public ::Task {
  public:
    TimedTasks() : ::Task(Timer::Task::priority + 1) { }
    void run();
  } timed_tasks;
  
public:
  enum { frequency = 100, ocr2a = F_CPU / 1024 / frequency - 1 };
  static const float dt;
  
  Timer();
  void init() { }
  
  void interrupt();
  
  class Task : public ::Task {
  public:
    enum { priority = 100 };
    Task() : ::Task(priority) { }
  };
  
  class Interval {
  private:
    long int value;
    
  public:
    Interval(unsigned long int value = 0) : value(value) { }
    long int seconds() const;
    long int microseconds() const;
    
    const Interval& operator +=(const Interval& rhs) { value += rhs.value; return *this; }
    const Interval operator +(const Interval& rhs) { return Interval(*this) += rhs; }

    const Interval& operator -=(const Interval& rhs) { value -= rhs.value; return *this; }
    const Interval operator -(const Interval& rhs) { return Interval(*this) -= rhs; }
  };
  
  class Stamp {
  private:
    unsigned long int value;
    
  public:
    Stamp();
    void operator ()();
    bool since(long int duration_in_seconds) const;

    bool operator >(const Stamp& rhs) const { return value > rhs.value; }
    bool operator <(const Stamp& rhs) const { return value < rhs.value; }
    
    const Interval operator -(const Stamp& rhs) { return Interval(value - rhs.value); }
  };

  unsigned long int stamp();
  
  class Diagnostic {
  private:
    Stamp start;
    const char *message;
    const bool report;
  public:
    Diagnostic(const char *message, int seconds = 0);
    ~Diagnostic();
  };
};

#endif
