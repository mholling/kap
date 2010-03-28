#include "eeprom.h"
#include <avr/io.h>
#include "app.h"

// unsigned int Eeprom::Packet::fletcher8() {
//   unsigned char a = 0x00;
//   unsigned char b = 0x00;
//   for (unsigned int n = 0; n < length; n++) {
//     a += buffer[n];
//     b += a;
//   }
//   return (a << 8) + b;
// }

unsigned int Eeprom::Packet::crc16() {
  static const unsigned int crc_poly = 0x8005;
  unsigned int result = 0;
  for (unsigned int j = 0; j < length; j++) {
    unsigned long m = (static_cast<unsigned long>(result) << 8) | buffer[j];
    for (int n = 0; n < 8; n++)
      if ((m <<= 1) & 0x1000000)
        m ^= (static_cast<unsigned long>(crc_poly) << 8);
    result = static_cast<unsigned int>(m >> 8);
  }
  return result;
}

void Eeprom::ReadPacket::before_dequeue() {
  valid = checksum == crc16();
}

void Eeprom::WritePacket::after_enqueue() {
  checksum = crc16();
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
  if (index >= length + sizeof(checksum)) return true;
  char& byte = index < length ? buffer[index] : reinterpret_cast<char *>(&checksum)[index - length];
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
