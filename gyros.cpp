#include "gyros.h"
#include "app.h"

Gyros::Gyros() : fixed(0.5), yaw(app.analog.yaw, fixed, 300.0), pitch(app.analog.pitch, app.analog.ref, 400.0), roll(app.analog.roll, app.analog.ref, 400.0) { }

void Gyros::init() {
  normal_mode();
  enable(); 
}

void Gyros::disable() {
  app.shift_register.set_bit(power_down_shift_register_bit);
}

void Gyros::enable() {
  app.shift_register.clear_bit(power_down_shift_register_bit);
}

void Gyros::test_mode() {
  app.shift_register.set_bit(self_test_shift_register_pin);
}

void Gyros::normal_mode() {
  app.shift_register.clear_bit(self_test_shift_register_pin);
}

float Gyros::Gyro::operator ()() const {
  CriticalSection cs;
  return (value() / reference() - 1.0) * range;
}

void Gyros::Task::run() {
  app.analog.start_conversions();
  for (bool done = false; done; done = !app.gyros.yaw.pending()) { } // TODO: can we have tasks suspend themselves instead?
  app.serial.debug("  yaw", (int)app.gyros.yaw());
}
