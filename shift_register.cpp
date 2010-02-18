#include "shift_register.h"
#include <avr/io.h>
#include "app.h"

ShiftRegister::ShiftRegister() : bits(0) {
  DDRD |= _BV(DDD4);
  PORTD |= _BV(PIND4);
}

void ShiftRegister::init() volatile {
  volatile Bit clear(0,0);
  clear(true);
}

void ShiftRegister::Bit::toggle_select() {
  PORTD ^= _BV(PIND4);
}

void ShiftRegister::Bit::after_enqueue() {
  data = set ? (app.shift_register.bits |= mask) : (app.shift_register.bits &= ~mask);
}
