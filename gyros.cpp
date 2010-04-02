#include "gyros.h"
#include "app.h"

Gyros::Gyros() :
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false),
  yaw(channels.yaw, channels.fixed, 300.0, &Attitude::Measure::yaw, 2.2e-5),
  pitch(channels.pitch, channels.ref, 400.0, &Attitude::Measure::pitch, 9.0e-6),
  roll(channels.roll, channels.ref, -400.0, &Attitude::Measure::roll, 5.0e-6) { }

Gyros::Gyro::Gyro(Analog::Channel& value, Analog::Channel& reference, float range, Attitude::Measure::angle_method_type angle_method, float angle_variance) :
  value(value),
  reference(reference),
  range(range),
  estimate(*this, angle_method, angle_variance) { }

float Gyros::Gyro::rate() const {
  reference.wait();
  value.wait();
  return (value() / reference() - 1.0) * range;
}

Gyros::Gyro::Estimate::Estimate(const Gyro& gyro, Attitude::Measure::angle_method_type angle_method, float angle_variance) :
   gyro(gyro),
   angle_method(angle_method)
   // q1(0.2 / Timer::frequency), q2(0.2), q3(0.1), // TODO: values??
{
  R(0,0) = 0.3;

  Q(0,0) = 0.25 / (Timer::frequency * Timer::frequency);
  Q(1,1) = 0.003;

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
