#include "shift_register.h"
#include <avr/io.h>

ShiftRegister::ShiftRegister() {
  DDRD |= _BV(DDD6);
  PORTD |= _BV(PIND6);
}

void ShiftRegister::init() {
}

inline const void ShiftRegister::SetValue::toggle_select() {
  PORTD ^= _BV(PIND6);
}

void ShiftRegister::SetValue::operator ()(unsigned char value) {
  data = value;
  enqueue();
}