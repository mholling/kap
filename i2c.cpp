#include "app.h"
#include "i2c.h"
#include <avr/io.h>
#include <util/twi.h>
#include "serial.h"

I2C::I2C() {
  PORTC |= _BV(PINC4) | _BV(PINC5); // set them high to enable pullups
  TWSR &= ~_BV(TWPS0) & ~_BV(TWPS1); // set pre-scaler to 1
  TWBR = ((F_CPU / frequency) - 16) / 2; // set bit-rate to 400 kbps
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

void I2C::Packet::initiate() {
  index = 0;
  start();
}

void I2C::Packet::terminate() {
  stop();
}

bool I2C::Packet::process() {
  bool complete = false;
  switch (TW_STATUS) {
    case TW_START:     // sent start condition
      TWDR = TW_WRITE | (address << 1);
      ack();
      break;
    case TW_REP_START: // sent repeated start condition
      TWDR =  TW_READ | (address << 1);
      ack();
      break;

    case TW_MT_SLA_ACK:  // slave receiver acked address
      TWDR = reg;
      ack();
      break;
    case TW_MT_DATA_ACK: // slave receiver acked data
      if (read_write == read)
        start();
      else if (index < length) {
        TWDR = buffer[index++];
        ack();
      } else complete = true;
      break;
  
    case TW_MR_DATA_ACK: // data received, ack sent
      buffer[index++] = TWDR;
    case TW_MR_SLA_ACK:  // address sent, ack received
      index == length - 1 ? nack() : ack();
      break;
  
    case TW_MR_DATA_NACK: // data received, nack sent
      buffer[index++] = TWDR;
      complete = true;
      break;
      
    case TW_MR_SLA_NACK: // address sent, nack received
    case TW_MT_SLA_NACK:  // address sent, nack received
    case TW_MT_DATA_NACK: // data sent, nack received
      stop();
      start();
      break;
    
    case TW_MT_ARB_LOST: // lost bus arbitration
      start();
      break;
  }
  return complete;
}

const void I2C::Packet::start() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTA);
}

const void I2C::Packet::stop() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO);
  while (TWCR & _BV(TWSTO)) ;
}

const void I2C::Packet::ack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

const void I2C::Packet::nack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

const void I2C::Packet::release() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

ISR(TWI_vect) {
  app.i2c.interrupt();
}
