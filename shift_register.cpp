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
  while (send_data.pending()) ; // block until any previous SPI packets are sent
  data |= (1 << n);
  send_data();
}

void ShiftRegister::clear_bit(unsigned char n) {
  while (send_data.pending()) ; // block until any previous SPI packets are sent
  data &= ~(1 << n);
  send_data();
}

inline const void ShiftRegister::SendData::toggle_select() {
  PORTD ^= _BV(PIND6);
}
