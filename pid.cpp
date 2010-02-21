#include "pid.h"
#include "app.h"

Pid::Pid() : yaw(app.gyros.yaw, app.trajectory.yaw, app.motors.yaw), pitch(app.gyros.pitch, app.trajectory.pitch, app.motors.pitch) { }

void Pid::Controller::run() {
  state.update();
  motor.set(kp * state.error + ki * state.integral + kd * state.derivative);
}

void Pid::Controller::State::reset() {
  error = derivative = integral = 0.0;
  previous = trajectory_angle();
}

void Pid::Controller::State::update() {
  error = trajectory_angle() - gyro.estimate.angle();
  derivative = (trajectory_angle() - previous) * Timer::frequency - gyro.estimate.rate();
  integral += error / Timer::frequency;
  previous = trajectory_angle();
}
