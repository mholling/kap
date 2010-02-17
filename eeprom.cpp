#include "eeprom.h"
#include <avr/io.h>
#include "app.h"
#include "safe.h"

bool Eeprom::Packet::valid() {
  char test = checksum;
  for (unsigned int n = 0; n < length; test ^= buffer[n++]) ;
  return test == 0;
  // TODO: use a better checksum algorithm?
}

bool Eeprom::Packet::operator ()(operation_value op, bool block) volatile {
  if ((*Safe<Packet>(this))(op)) {
    if (block) wait();
    return true;
  } else return false;
}

bool Eeprom::Packet::operator ()(operation_value op) {
  if (pending()) return false;
  operation = op;
  return enqueue();
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
  if (index > length) return true;
  char& byte = index == length ? checksum : buffer[index];
  EEAR = address + index++;
  switch (operation) {
    case reading:
      EECR |= _BV(EERE);
      byte = EEDR;
      break;
    case writing:
      EEDR = byte;
      EECR |= _BV(EEMPE);
      EECR |= _BV(EEPE);
  }
  return false;
}


ISR(EE_READY_vect) {
  app.eeprom.interrupt();
}
