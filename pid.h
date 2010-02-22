#ifndef __PID_H_
#define __PID_H_

#include "scheduler.h"
#include "gyros.h"
#include "motors.h"
#include "trajectory.h"

class Pid {
public:
  Pid();
  void init() { yaw.init(); pitch.init(); }
  
  class Controller : public Timer::Task {
  private:
    const Gyros::Gyro& gyro;
    const Trajectory::Angle target;
    Motors::Motor& motor;

    float previous;
    float integral;
      
    const float kp;
    const float ti;
    const float td;
    const float clamp;
    
  public:
    Controller(const Gyros::Gyro& gyro, const Trajectory::Angle target, Motors::Motor& motor) :
      gyro(gyro),
      target(target),
      motor(motor),
      kp(1.0), ti(0.2), td(0.0), clamp(ti / kp) { reset(); }
    void init() { }
    
    void run();

    void reset();
    void update();
  };
  
  Controller yaw;
  Controller pitch;
};

#endif
