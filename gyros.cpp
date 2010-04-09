#include "gyros.h"
#include "app.h"
#include "timer.h"
#include "vector.h"

Gyros::Gyros() :
  x(channels.x, channels.ref, 400.0 * M_PI / 180.0),
  y(channels.y, channels.ref, -400.0 * M_PI / 180.0),
  z(channels.z, channels.fixed, -300.0 * M_PI / 180.0),
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false) { }

Gyros::Gyro::Gyro(Analog::Channel& value, Analog::Channel& reference, float range) :
  value(value),
  reference(reference),
  range(range) { }

float Gyros::Gyro::rate() const {
  reference.wait();
  value.wait();
  return (value() / reference() - 1.0) * range;
}

void Gyros::measure() {
  channels.z.measure();
  channels.y.measure();
  channels.x.measure();
  channels.ref.measure();
}

Vector Gyros::rates() const {
  Vector result(x.rate(), y.rate(), z.rate());
  app.orientation.adjust(result);
  return result;
}
