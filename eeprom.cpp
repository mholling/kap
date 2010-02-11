#include "eeprom.h"
#include "critical_section.h"
#include <avr/io.h>
#include "app.h"

void Eeprom::Packet::operator ()(operation_value op, bool block) {
  wait();
  {
    CriticalSection cs;
    operation = op;
    Queable<Packet>::enqueue(); // TODO: qualifier needed?
    if (at_head()) start();
    volatile int m = 20000; // TODO: fix volatiles to remove this...
  }
  if (block) wait();
}

void Eeprom::Packet::dequeue() {
  CriticalSection cs;
  Queable<Packet>::dequeue();
  if (any()) head().start();
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
