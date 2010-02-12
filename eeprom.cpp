#include "eeprom.h"
#include <avr/io.h>
#include "app.h"
#include "safe.h"

void Eeprom::Packet::operator ()(operation_value op, bool block) volatile {
  wait();
  { Safe<Packet>(this)()(op); }
  if (block) wait();
}

void Eeprom::Packet::operator ()(operation_value op) {
  operation = op;
  enqueue();
  if (at_head()) start();
  // volatile int m = 20000; // TODO: fix volatiles to remove this...
}

void Eeprom::Packet::dequeue() {
  Queable<Packet>::dequeue();
  if (any()) const_cast<Packet&>(head()).start();
}

void Eeprom::Packet::start() {
  index = 0;
  EECR |= _BV(EERIE);
}

void Eeprom::Packet::stop() {
  index = 0;
  EECR &= ~_BV(EERIE);
}

void Eeprom::Packet::interrupt() {
  EEAR = address + index;
  if (index < length) {
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
  } else {
    stop();
    dequeue();
  }
}

ISR(EE_READY_vect) {
  Eeprom::Packet::head().interrupt();
}
