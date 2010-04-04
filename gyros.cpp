#include "gyros.h"
#include "app.h"

Gyros::Gyros() :
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false),
  yaw(channels.yaw, channels.fixed, -300.0 * 3.141592653589793 / 180.0, &Attitude::Measure::yaw, 0.6, 0.015, 0.1),
  pitch(channels.pitch, channels.ref, -400.0 * 3.141592653589793 / 180.0, &Attitude::Measure::pitch, 0.5, 0.008, 0.1),
  roll(channels.roll, channels.ref, -400.0 * 3.141592653589793 / 180.0, &Attitude::Measure::roll, 0.5, 0.008, 0.1) { }

Gyros::Gyro::Gyro(Analog::Channel& value, Analog::Channel& reference, float range, Attitude::Measure::angle_method_type angle_method, float angle_sd, float rate_sd, float bias_sd) :
  value(value),
  reference(reference),
  range(range),
  estimate(*this, angle_method, angle_sd, rate_sd, bias_sd) { }

float Gyros::Gyro::rate() const {
  reference.wait();
  value.wait();
  return (value() / reference() - 1.0) * range;
}

Gyros::Gyro::Estimate::Estimate(const Gyro& gyro, Attitude::Measure::angle_method_type angle_method, float angle_sd, float rate_sd, float bias_sd) :
   gyro(gyro),
   angle_method(angle_method)
{
  R(0,0) = angle_sd * angle_sd; // variance of angle jitter due to accelerometer
  
  // TODO: have values for Q vary according to whether motors are going or not?
  
  Matrix<2, 1> q;
  q(0,0) = rate_sd / Timer::frequency;
  q(1,0) = bias_sd; // standard deviation of bias
  Q = q * q.t();

  A(0,0) = A(1,1) = 1.0;
  A(0,1) = -1.0 / Timer::frequency;
  At = A.t();

  B(0,0) = -1.0 / Timer::frequency;

  H(0,0) = 1.0;
  Ht = H.t();
}

void Gyros::Gyro::Estimate::run() {
  u(0,0) = gyro.rate();
  z(0,0) = (app.attitude.measure.*angle_method)();
  predict();
  correct();
}

void Gyros::Gyro::Estimate::predict() {
  // compute  x = A x + B u
  x = A * x + B * u;
  
  // compute  P = A P A' + Q
  P = A * P * At + Q;
}

void Gyros::Gyro::Estimate::correct() {  
  // compute Kalman gain  K = P H' / (H P H' + R)
  Matrix<2, 1> K = (P * Ht) / (H * P * Ht + R);
  
  // compute  x += K (z - H x)
  x += K * (z - H * x);
  
  // compute  P -= K H P
  P -= K * H * P;
}
