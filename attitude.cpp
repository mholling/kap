#include "attitude.h"
#include "app.h"
#include "vector.h"
#include "quaternion.h"

void Attitude::Measure::run() {
  app.accelerometer.measure.wait();
    
  const Vector b1 = app.orientation.adjust_vector(app.accelerometer.vector()).normalised();          // gravity
  const Vector b2 = b1.cross(app.orientation.adjust_vector(app.magnetometer.vector())).normalised(); // magnetic west
  const Vector b3 = b1.cross(b2);
  
  const Vector b1r1a1_plus_b2r2a2(a2 * b2[2] - a1 * b1[1], a1 * b1[0], -a2 * b2[0]);
  const Vector b3r3(0.0, -b3[2], b3[1]);
  const Vector b3_plus_r3(b3[0] - 1.0, b3[1], b3[2]);
  const float b3_dot_r3_plus_1 = 1.0 - b3[0];
  
  const float alpha = -b3_dot_r3_plus_1 * (a1 * b1[2] + a2 * b2[1]) + b3r3.dot(b1r1a1_plus_b2r2a2);
  const float beta = b3_plus_r3.dot(b1r1a1_plus_b2r2a2);
  const float gamma = sqrt(alpha * alpha + beta * beta);
  
  if (alpha >= 0) {
    quaternion = Quaternion(b3r3 * (gamma + alpha) + b3_plus_r3 * beta, (gamma + alpha) * b3_dot_r3_plus_1);
    quaternion /= 2 * sqrt(gamma * (gamma + alpha) * b3_dot_r3_plus_1);
  } else {
    quaternion = Quaternion(b3r3 * beta + b3_plus_r3 * (gamma - alpha), beta * b3_dot_r3_plus_1);
    quaternion /= 2 * sqrt(gamma * (gamma - alpha) * b3_dot_r3_plus_1);
  }
}

void Attitude::Estimate::init() {
  Vector angle_variance = app.orientation.adjust_variance(square(0.5),   square(0.5),   square(0.9));
  Vector rate_variance  = app.orientation.adjust_variance(square(0.008), square(0.008), square(0.012));
  Vector bias_variance  = app.orientation.adjust_variance(square(0.12),   square(0.12),   square(0.1));
  
  roll.set_variances(angle_variance[0], rate_variance[0], bias_variance[0]);
  pitch.set_variances(angle_variance[1], rate_variance[1], bias_variance[1]);
  yaw.set_variances(angle_variance[2], rate_variance[2], bias_variance[2]);
  
 // TODO: eventually, initialize Kalman states to measured attitude
}

void Attitude::Estimate::run() {
  Vector rates = app.orientation.adjust_vector(app.gyros.rates());
  
  yaw.filter(app.attitude.measure.yaw(), rates[0]);
  pitch.filter(app.attitude.measure.pitch(), rates[1]);
  roll.filter(app.attitude.measure.roll(), rates[2]);
}

void Attitude::Estimate::Kalman::set_variances(float angle_variance, float rate_variance, float bias_variance) {
  Q11 = rate_variance * Timer::dt * Timer::dt;
  Q12 = sqrt(Q11 * bias_variance); // 0.0; // TODO?
  Q22 = bias_variance;
  R   = angle_variance;
}

void Attitude::Estimate::Kalman::filter(float measured_angle, float measured_rate) {
  if (measured_angle < old_angle - M_PI) revolutions++;
  if (measured_angle > old_angle + M_PI) revolutions--;
  old_angle = measured_angle;
  
  measured_angle += revolutions * 2 * M_PI;
    
  // compute:  x = A x + B u
  x1 += (measured_rate - x2) * Timer::dt;
  
  // compute:  P = A P A' + Q
  P11 -= P12 * Timer::dt;
  P12 -= P22 * Timer::dt;
  P11 -= P12 * Timer::dt;
  P11 += Q11;
  P12 += Q12;
  P22 += Q22;

  // compute Kalman gain:  S = H P H' + R,  K = P H' / S
  float s = P11 + R;
  
  // compute:  x += K (z - H x)
  float y_s = (measured_angle - x1) / s;
  x1 += P11 * y_s;
  x2 += P12 * y_s;
  
  // compute:  P -= K H P
  P22 -= P12 * P12 / s;
  P12 -= P12 * P11 / s;
  P11 -= P11 * P11 / s;
}
