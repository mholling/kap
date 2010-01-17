#include "spi.h"
#include "app.h"

Spi::Spi() {
  SPCR = _BV(SPIE) | _BV(SPE) | _BV(MSTR); // enable SPI in master mode with interrupts, SCK freq = F_CPU/4
  DDRB |= _BV(DDB3) | _BV(DDB5); // set MOSI and SCK as outputs
}

void Spi::init() {
}

const void Spi::interrupt() {
  Message::head().next();
}

void Spi::Message::enqueue() {
  index = 0;
  Queable<Message>::enqueue();
  if (at_head()) start();
}

void Spi::Message::start() {
  index = 0;
  toggle_select();
  SPDR = index < tx_length ? tx_buffer[index] : 0;
}

void Spi::Message::next() {
  if (index < rx_length) rx_buffer[index] = SPDR;
  index++;
  if (index < tx_length || index < rx_length)
    SPDR = index < tx_length ? tx_buffer[index] : 0;
  else {
    CriticalSection cs;
    toggle_select();
    dequeue();
    if (any()) head().start();
  }
}

ISR(SPI_STC_vect) {
  App::app().spi.interrupt();
}
