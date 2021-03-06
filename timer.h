#ifndef __TIMER_H_
#define __TIMER_H_

#include "task.h"

class Timer {
private:
  unsigned long int count;
  int steps;
  
    
public:
  enum { frequency = 90, ocr2a = F_CPU / 1024 / frequency - 1 };
  
  Timer();
  void init();
  
  void interrupt();
  
  class Task : public ::Task {
  protected:
    float dt;
    
  public:
    enum { priority = 100 };
    Task() : ::Task(priority) { }
    
    static bool any();
    void operator ()(float _dt);    
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
