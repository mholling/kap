#include "gyros.h"
#include "app.h"
#include "safe.h"

Gyros::Gyros() : yaw_channel(0), pitch_channel(1), roll_channel(2), ref_channel(3), fixed_channel(0.5), yaw(yaw_channel, fixed_channel, 300.0), pitch(pitch_channel, ref_channel, 400.0), roll(roll_channel, ref_channel, 400.0) { }

void Gyros::disable() volatile {
  app.shift_register.set_bit(power_down_shift_register_bit);
}

void Gyros::enable() volatile {
  app.shift_register.clear_bit(power_down_shift_register_bit);
}

void Gyros::test_mode() volatile {
  app.shift_register.set_bit(self_test_shift_register_pin);
}

void Gyros::normal_mode() volatile {
  app.shift_register.clear_bit(self_test_shift_register_pin);
}

void Gyros::measure() {
  yaw_channel.convert();
  pitch_channel.convert();
  roll_channel.convert();
  ref_channel.convert();
}

float Gyros::Gyro::operator ()() const volatile { return Safe<const Gyro>(this)()(); }

float Gyros::Gyro::operator ()() const {
  return (value() / reference() - 1.0) * range;
}
