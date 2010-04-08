#ifndef __PID_H_
#define __PID_H_

#include "task.h"
#include "attitude.h"
#include "motors.h"
#include "trajectory.h"

class Pid {
public:
  Pid();
  void init() { yaw.init(); pitch.init(); }
  
  class Controller : public Timer::Task {
  private:
    const Attitude::Estimate::Kalman actual;
    const Trajectory::Angle target;
    Motors::Motor& motor;

    float previous;
    float integral;
      
    const float kp;
    const float ti;
    const float td;
    const float clamp;
    
  public:
    Controller(const Attitude::Estimate::Kalman actual, const Trajectory::Angle target, Motors::Motor& motor) :
      actual(actual),
      target(target),
      motor(motor),
      kp(1.0), ti(0.2), td(0.0), clamp(ti / kp) { }
    void init() { reset(); }
    
    void run();
    void reset();
    void update();
  };
  
  Controller yaw, pitch;
};

#endif
