#include "shift_register.h"
#include <avr/io.h>

ShiftRegister::ShiftRegister() : data(0), send_data(data) {
  DDRD |= _BV(DDD6);
  PORTD |= _BV(PIND6);
}

void ShiftRegister::init() {
  send_data();
}

void ShiftRegister::set_bit(unsigned char n) {
  data |= (1 << n);
  // send_data();
  // for (bool done = false; done; done = !send_data.pending()) { }
  send_data(true);
}

void ShiftRegister::clear_bit(unsigned char n) {
  data &= ~(1 << n);
  // send_data();
  // for (bool done = false; done; done = !send_data.pending()) { }
  send_data(true);
}

inline const void ShiftRegister::SendData::toggle_select() {
  PORTD ^= _BV(PIND6);
}
