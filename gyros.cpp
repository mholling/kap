#include "gyros.h"
#include "app.h"

Gyros::Gyros() : fixed(0.5), yaw(app.analog.yaw, fixed, 300.0), pitch(app.analog.pitch, app.analog.ref, 400.0), roll(app.analog.roll, app.analog.ref, 400.0) { }

void Gyros::disable() volatile {
  // app.shift_register.set_bit(power_down_shift_register_bit);
}

void Gyros::enable() volatile {
  // app.shift_register.clear_bit(power_down_shift_register_bit);
}

void Gyros::test_mode() volatile {
  // app.shift_register.set_bit(self_test_shift_register_pin);
}

void Gyros::normal_mode() volatile {
  // app.shift_register.clear_bit(self_test_shift_register_pin);
}

void Gyros::measure() volatile {
  app.analog.yaw.convert();
  app.analog.pitch.convert();
  app.analog.roll.convert();
  app.analog.ref.convert();
}

float Gyros::Gyro::operator ()() const volatile {
  return (value() / reference() - 1.0) * range;
}
