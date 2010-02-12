#include "shift_register.h"
#include <avr/io.h>

ShiftRegister::ShiftRegister() : data(0), send_data(data) {
  DDRD |= _BV(DDD4);
  PORTD |= _BV(PIND4);
}

void ShiftRegister::set_bit(unsigned char n) {
  data |= (1 << n);
  send_data(true);
}

void ShiftRegister::clear_bit(unsigned char n) {
  data &= ~(1 << n);
  send_data(true);
}

inline const void ShiftRegister::SendData::toggle_select() {
  PORTD ^= _BV(PIND4);
}
