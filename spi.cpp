#include "spi.h"
#include "app.h"
#include "critical_section.h"

Spi::Spi() {
  SPCR = _BV(SPIE) | _BV(SPE) | _BV(MSTR); // enable SPI in master mode with interrupts, SCK freq = F_CPU/4
  DDRB |= _BV(DDB3) | _BV(DDB5); // set MOSI and SCK as outputs
}

const void Spi::interrupt() {
  Packet::head().interrupt();
}

void Spi::Packet::enqueue() {
  CriticalSection cs;
  index = 0;
  Queable<Packet>::enqueue();
  if (at_head()) start();
}

void Spi::Packet::dequeue() {
  CriticalSection cs;
  Queable<Packet>::dequeue();
  if (any()) head().start();
}

void Spi::Packet::start() {
  index = 0;
  toggle_select();
  SPDR = index < tx_length ? tx_buffer[index] : 0;
}

void Spi::Packet::interrupt() {
  if (index < rx_length) rx_buffer[index] = SPDR;
  index++;
  if (index < tx_length || index < rx_length)
    SPDR = index < tx_length ? tx_buffer[index] : 0;
  else {
    toggle_select();
    dequeue();
  }
}

ISR(SPI_STC_vect) {
  app.spi.interrupt();
}
