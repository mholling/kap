#include "shift_register.h"
#include <avr/io.h>

ShiftRegister::ShiftRegister() : data(0), send_data(SendData(data)) {
  DDRD |= _BV(DDD6);
  PORTD |= _BV(PIND6);
}

void ShiftRegister::init() {
}

void ShiftRegister::set_bit(int n) {
  while (send_data.pending()) ; // block until the SPI packet is not enqueued
  data |= (1 << n);
  send_data();
}

void ShiftRegister::clear_bit(int n) {
  while (send_data.pending()) ; // block until the SPI packet is not enqueued
  data &= ~(1 << n);
  send_data();
}

inline const void ShiftRegister::SendData::toggle_select() {
  PORTD ^= _BV(PIND6);
}
