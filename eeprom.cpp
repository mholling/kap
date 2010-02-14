#include "eeprom.h"
#include <avr/io.h>
#include "app.h"
#include "safe.h"

void Eeprom::Packet::operator ()(operation_value op, bool block) volatile {
  wait();
  operation = op;
  enqueue(block);
}

void Eeprom::Packet::initiate() {
  index = 0;
  EECR |= _BV(EERIE);
}

void Eeprom::Packet::terminate() {
  index = 0;
  EECR &= ~_BV(EERIE);
}

bool Eeprom::Packet::process() {
  if (index >= length) return true;
  EEAR = address + index;
  switch (operation) {
    case reading:
      EECR |= _BV(EERE);
      buffer[index++] = EEDR;
      break;
    case writing:
      EEDR = buffer[index++];
      EECR |= _BV(EEMPE);
      EECR |= _BV(EEPE);
  }
  return false;
}

ISR(EE_READY_vect) {
  Eeprom::Packet::head().interrupt();
}
