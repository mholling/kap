#include "pid.h"
#include "app.h"

Pid::Pid() : yaw(app.gyros.yaw, app.trajectory.yaw, app.motors.yaw), pitch(app.gyros.pitch, app.trajectory.pitch, app.motors.pitch) { }

void Pid::Controller::run() {
  float error = target() - gyro.estimate.angle();
  float derivative = (target() - previous) / Timer::dt - gyro.estimate.rate();
  integral += error * Timer::dt;
  if (integral >  clamp) integral =  clamp;
  if (integral < -clamp) integral = -clamp;
  previous = target();
  
  motor.set(kp * (error + integral / ti + derivative * td));
}

void Pid::Controller::reset() {
  integral = 0.0;
  previous = target();
}
