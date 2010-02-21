#ifndef __TRAJECTORY_H_
#define __TRAJECTORY_H_

#include "timer.h"

class Trajectory : public Timer::Task {
public:
  class Angle {
  private:
    float target;
    
  public:
    Angle() : target(0.0) { }
    float operator ()() const { return target; }
  };
  
  Angle yaw;
  Angle pitch;
  
  Trajectory() { }
  void init() { }
  void run();
};

#endif
