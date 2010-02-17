#include "spi.h"
#include <avr/io.h>
#include "app.h"

Spi::Spi() {
  SPCR = _BV(SPIE) | _BV(SPE) | _BV(MSTR);  // enable SPI in master mode with interrupts, SCK freq = F_CPU/4
  DDRB |= _BV(DDB3) | _BV(DDB5);            // set MOSI and SCK as outputs
}

void Spi::Packet::initiate() {
  index = 0;
  toggle_select();
  SPDR = index < tx_length ? tx_buffer[index] : 0;
}

inline void Spi::Packet::terminate() {
  toggle_select();
}

bool Spi::Packet::process() {
  if (index < rx_length) rx_buffer[index] = SPDR;
  index++;
  if (index < tx_length || index < rx_length) {
    SPDR = index < tx_length ? tx_buffer[index] : 0;
    return false;
  } else return true;
}

ISR(SPI_STC_vect) {
  app.spi.interrupt();
}
