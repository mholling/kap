#include "app.h"
#include "i2c.h"
#include <avr/io.h>
#include <util/twi.h>
#include "serial.h"
#include "critical_section.h"

I2C::I2C() {
  PORTC |= _BV(PINC4) | _BV(PINC5); // set them high to enable pullups
  TWSR &= ~(_BV(TWPS0) | _BV(TWPS1)); // set pre-scaler to 1
  TWBR = ((F_CPU / 100000) - 16) / 2; // set bit-rate to 400 kbps
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

void I2C::init() {
}

const void I2C::interrupt() {
  Message::head().interrupt();
}

const void I2C::Message::start() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTA);
}

const void I2C::Message::stop() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO);
  while (TWCR & _BV(TWSTO)) ;
}

const void I2C::Message::ack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

const void I2C::Message::nack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

const void I2C::Message::release() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

void I2C::Message::interrupt() {
  // App::app().serial.debug("TW_STATUS", (char)TW_STATUS);
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
      } else {
        stop();
        completed();
      }
      break;
  
    case TW_MR_DATA_ACK: // data received, ack sent
      buffer[index++] = TWDR;
    case TW_MR_SLA_ACK:  // address sent, ack received
      index == length - 1 ? nack() : ack();
      break;
  
    case TW_MR_DATA_NACK: // data received, nack sent
      buffer[index++] = TWDR;
      stop();
      completed();
      break;
      
    case TW_MR_SLA_NACK: // address sent, nack received
      stop();
      start();
      break;
  
    case TW_MT_SLA_NACK:  // address sent, nack received
      stop();
      start();
      break;
      
    case TW_MT_DATA_NACK: // data sent, nack received
      stop();
      start();
      break;
  
    case TW_MT_ARB_LOST: // lost bus arbitration
      start();
      break;
  }
}

const void I2C::Message::completed() {
  CriticalSection cs;
  dequeue();
  if (any()) head().start();
}

const void I2C::new_message() {
  if (Message::just_one()) Message::start();
}
    
void I2C::Message::enqueue() {
  index = 0;
  Queable<Message>::enqueue();
  App::app().i2c.new_message();
}

ISR(TWI_vect) {
  App::app().i2c.interrupt();
}
