#include "estimate_attitude.h"
#include "app.h"
#include "quaternion.h"
#include "serial.h"

void EstimateAttitude::run() {
  app.accelerometer.measure.wait();
  app.magnetometer.measure.wait();
  
  const Vector b1 = app.accelerometer.measure.vector.normalised(); // gravity
  const Vector b2 = b1.cross(app.magnetometer.measure.vector).normalised(); // magnetic west
  const Vector b3 = b1.cross(b2);
  
  const Vector b1r1a1_plus_b2r2a2(a2 * b2(2) - a1 * b1(1), a1 * b1(0), -a2 * b2(0));
  const Vector b3r3(0.0, -b3(2), b3(1));
  const Vector b3_plus_r3(b3(0) - 1.0, b3(1), b3(2));
  const float b3_dot_r3_plus_1 = 1.0 - b3(0);

  const float alpha = -b3_dot_r3_plus_1 * (a1 * b1(2) + a2 * b2(1)) + b3r3.dot(b1r1a1_plus_b2r2a2);
  const float beta = b3_plus_r3.dot(b1r1a1_plus_b2r2a2);
  const float gamma = sqrt(alpha * alpha + beta * beta);

  // if (alpha >= 0) {
  //   Quaternion quaternion(b3r3 * (gamma + alpha) + b3_plus_r3 * beta, (gamma + alpha) * b3_dot_r3_plus_1);
  //   quaternion /= 2 * sqrt(gamma * (gamma + alpha) * b3_dot_r3_plus_1);
  //   quaternion.get_euler_angles(yaw, pitch, roll);
  // } else {
  //   Quaternion quaternion(b3r3 * beta + b3_plus_r3 * (gamma - alpha), beta * b3_dot_r3_plus_1);
  //   quaternion /= 2 * sqrt(gamma * (gamma - alpha) * b3_dot_r3_plus_1);
  //   quaternion.get_euler_angles(yaw, pitch, roll);
  // }
}
