#include "gyros.h"
#include "app.h"
#include "timer.h"

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
  r = angle_sd * angle_sd;
  Q11 = rate_sd * rate_sd / (Timer::frequency * Timer::frequency);
  Q12 = rate_sd * bias_sd / Timer::frequency;
  Q22 = bias_sd * bias_sd;
}


void Gyros::Gyro::Estimate::run() {
  // compute  x = A x + B u
  x1 += (gyro.rate() - x2) / Timer::frequency;
  
  // compute  P = A P A' + Q
  P11 -= P12 / Timer::frequency;
  P12 -= P22 / Timer::frequency;
  P11 -= P12 / Timer::frequency;
  P11 += Q11;
  P12 += Q12;
  P22 += Q22;

  // compute Kalman gain  S = H P H' + R,  K = P H' / S
  float s = P11 + r;
  
  // compute  x += K (z - H x)
  float y_s = ((app.attitude.measure.*angle_method)() - x1) / s;
  x1 += P11 * y_s;
  x2 += P12 * y_s;
  
  // compute  P -= K H P
  P22 -= P12 * P12 / s;
  P12 -= P12 * P11 / s;
  P11 -= P11 * P11 / s;
}
