#include "attitude.h"
#include "app.h"
#include "timed_section.h"
#include "quaternion.h"
#include "serial.h"

void Attitude::EstimateTask::run() {
  app.accelerometer.measure(true);
  app.magnetometer.measure(true);
  
  do {
    TimedSection ts("estimate");
    const Vector b1 = app.accelerometer.vector().normalised(); // gravity
    const Vector b2 = (b1 * app.magnetometer.vector()).normalised(); // magnetic west
    const Vector b3 = b1 * b2;
    
    // const Vector b1r1a1_plus_b2r2a2 = b1 * r1 * a1 + b2 * r2 * a2;
    // const Vector b3r3 = b3 * r3;
    // const Vector b3_plus_r3 = b3 + r3;
    // const float b3_dot_r3_plus_1 = 1.0 + b3 % r3;
    // const float alpha = b3_dot_r3_plus_1 * (b1 % r1 * a1 + b2 % r2 * a2) + b3r3 % b1r1a1_plus_b2r2a2;
    // const float beta = b3_plus_r3 % b1r1a1_plus_b2r2a2;
    // const float gamma = sqrt(alpha * alpha + beta * beta);

    const Vector b1r1a1_plus_b2r2a2(a2 * b2.z - a1 * b1.y, a1 * b1.x, -a2 * b2.x);
    const Vector b3r3(0.0, -b3.z, b3.y);
    const Vector b3_plus_r3(b3.x - 1.0, b3.y, b3.z);
    const float b3_dot_r3_plus_1 = 1.0 - b3.x;
  
    const float alpha = -b3_dot_r3_plus_1 * (a1 * b1.z + a2 * b2.y) + b3r3 % b1r1a1_plus_b2r2a2;
    const float beta = b3_plus_r3 % b1r1a1_plus_b2r2a2;
    const float gamma = sqrt(alpha * alpha + beta * beta);
  
    if (alpha >= 0) {
      result = Quaternion(b3r3 * (gamma + alpha) + b3_plus_r3 * beta, (gamma + alpha) * b3_dot_r3_plus_1);
      result /= 2 * sqrt(gamma * (gamma + alpha) * b3_dot_r3_plus_1);
    } else {
      result = Quaternion(b3r3 * beta + b3_plus_r3 * (gamma - alpha), beta * b3_dot_r3_plus_1);
      result /= 2 * sqrt(gamma * (gamma - alpha) * b3_dot_r3_plus_1);
    }
  } while (false);
  
  app.serial.debug("yaw", result.yaw());
  app.serial.debug("pitch", result.pitch());
  app.serial.debug("roll", result.roll());
  app.serial.line();
}
// #include "attitude.h"
// #include "app.h"
// #include "timed_section.h"
// #include "quaternion.h"
// #include "serial.h"
// 
// void Attitude::EstimateTask::run() {
//   app.accelerometer.measure(true);
//   app.magnetometer.measure(true);
//   
//   do {
//     TimedSection ts("estimate");
//     const Vector b1 = app.accelerometer.vector().normalised(); // gravity
//     const Vector b2 = (b1 * app.magnetometer.vector()).normalised(); // magnetic west
//     const Vector b3 = b1 * b2;
//   
//     float a1 = 1.0, a2 = 2.0;
//     const Vector b1r1a1_plus_b2r2a2 = b1 * r1 * a1 + b2 * r2 * a2;
//     const Vector b3r3 = b3 * r3;
//     const Vector b3_plus_r3 = b3 + r3;
//     const float b3_dot_r3_plus_1 = 1.0 + b3 % r3;
//   
//     const float alpha = b3_dot_r3_plus_1 * (b1 % r1 * a1 + b2 % r2 * a2) + b3r3 % b1r1a1_plus_b2r2a2;
//     const float beta = b3_plus_r3 % b1r1a1_plus_b2r2a2;
//     const float gamma = sqrt(alpha * alpha + beta * beta);
//   
//     if (alpha >= 0) {
//       result = Quaternion(b3r3 * (gamma + alpha) + b3_plus_r3 * beta, (gamma + alpha) * b3_dot_r3_plus_1);
//       result /= 2 * sqrt(gamma * (gamma + alpha) * b3_dot_r3_plus_1);
//     } else {
//       result = Quaternion(b3r3 * beta + b3_plus_r3 * (gamma - alpha), beta * b3_dot_r3_plus_1);
//       result /= 2 * sqrt(gamma * (gamma - alpha) * b3_dot_r3_plus_1);
//     }
//   } while (false);
//   
//   app.serial.debug("yaw", result.yaw());
//   app.serial.debug("pitch", result.pitch());
//   app.serial.debug("roll", result.roll());
//   app.serial.line();
// }