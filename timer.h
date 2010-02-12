#ifndef __TIMER_H_
#define __TIMER_H_

#include "safe.h"

class Timer {
private:
  unsigned long int count;
  
public:
  enum { frequency = 35 };
  Timer();
  inline void init() volatile { }
  void interrupt();
  unsigned long int timestamp() volatile { return Safe<Timer>(this)().timestamp(); }
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
