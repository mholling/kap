#include "gyros.h"
#include "app.h"
#include "timer.h"
#include "vector.h"

Gyros::Gyros() :
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false),
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

Vector Gyros::rates() const {
  Vector result(x.rate(), y.rate(), z.rate());
  app.orientation.adjust(result);
  return result;
}
