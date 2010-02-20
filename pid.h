#ifndef __PID_H_
#define __PID_H_

#include "scheduler.h"
#include "gyros.h"
#include "motors.h"

class Pid {
public:
  Pid();
  void init() { yaw.init(); pitch.init(); }
  
  class Controller : public Timer::Task {
  private:
    const Gyros::Gyro& gyro;
    const Motors::Motor& motor;
  
  public:
    Controller(const Gyros::Gyro& gyro, const Motors::Motor& motor) : gyro(gyro), motor(motor) { }
    void init() { }
    
    void run();
  };
  
  Controller yaw;
  Controller pitch;
};

#endif
