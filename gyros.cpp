#include "gyros.h"
#include "app.h"

void Gyros::init() {
  normal_mode();
  enable(); 
}

void Gyros::disable() {
  app().shift_register.set_bit(power_down_shift_register_bit);
}

void Gyros::enable() {
  app().shift_register.clear_bit(power_down_shift_register_bit);
}

void Gyros::test_mode() {
  app().shift_register.set_bit(self_test_shift_register_pin);
}

void Gyros::normal_mode() {
  app().shift_register.clear_bit(self_test_shift_register_pin);
}

inline float Gyros::Gyro::operator ()() {
  CriticalSection cs;
  return (value - origin) * scale / 1024;
}