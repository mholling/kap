#ifndef __TIMER_H_
#define __TIMER_H_

class Timer {
public:
  enum { frequency = 50 };
  Timer();
  inline void init() { }
  void interrupt();
};

#endif
