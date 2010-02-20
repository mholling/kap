#include "pid.h"
#include "app.h"

Pid::Pid() : yaw(app.gyros.yaw, app.motors.yaw), pitch(app.gyros.pitch, app.motors.pitch) { }

void Pid::Controller::run() {
  // do stuff with motor and gyro.estimate here
}
