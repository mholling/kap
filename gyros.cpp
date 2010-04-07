#include "gyros.h"
#include "app.h"
#include "timer.h"
#include "vector.h"

Gyros::Gyros() :
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false),
  // z(channels.z, channels.fixed, -300.0 * M_PI / 180.0, &Attitude::Measure::yaw, 0.9, 0.012, 0.1),
  // y(channels.y, channels.ref, -400.0 * M_PI / 180.0, &Attitude::Measure::pitch, 0.5, 0.008, 0.1),
  // x(channels.x, channels.ref, 400.0 * M_PI / 180.0, &Attitude::Measure::roll, 0.5, 0.008, 0.1) { } // TODO: remove
  x(channels.x, channels.ref, 400.0 * M_PI / 180.0),
  y(channels.y, channels.ref, -400.0 * M_PI / 180.0),
  z(channels.z, channels.fixed, -300.0 * M_PI / 180.0) { }

Gyros::Gyro::Gyro(Analog::Channel& value, Analog::Channel& reference, float range) :
  value(value),
  reference(reference),
  range(range) { }

float Gyros::Gyro::rate() const {
  reference.wait();
  value.wait();
  return (value() / reference() - 1.0) * range;
}

const Vector Gyros::rates() const {
  Vector result;
  result[0] = x.rate();
  result[1] = y.rate();
  result[2] = z.rate();
  return result;
}
