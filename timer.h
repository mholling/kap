#ifndef __TIMER_H_
#define __TIMER_H_

class Timer {
private:
  enum { frequency = 50 };
public:
  Timer();
  inline void init() { }
  void interrupt();
};

#endif
