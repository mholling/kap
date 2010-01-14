#include "app.h"
#include "i2c.h"
#include <avr/io.h>
#include <util/twi.h>
#include "serial.h"

I2C::I2C() {
  PORTC |= _BV(PINC4) | _BV(PINC5); // set them high to enable pullups
  TWSR &= ~(_BV(TWPS0) | _BV(TWPS1)); // set pre-scaler to 1
  TWBR = ((F_CPU / 100000) - 16) / 2; // set bit-rate to 400 kbps
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

const void I2C::init() {
  
}

const void I2C::Task::start() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTA);
}

const void I2C::Task::stop() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO);
  while (TWCR & _BV(TWSTO)) ;
}

const void I2C::Task::ack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

const void I2C::Task::nack() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
}

const void I2C::Task::release() {
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}

const void I2C::interrupt() {
  TWCR &= ~(_BV(TWINT) | _BV(TWIE));
  App::app().scheduler.signal(task);
}

void I2C::Task::operator ()() {
  switch (TW_STATUS) {
    case TW_START:     // sent start condition
      TWDR = TW_WRITE | (Message::head().address << 1);
      ack();
      break;
    case TW_REP_START: // sent repeated start condition
      TWDR =  TW_READ | (Message::head().address << 1);
      ack();
      break;
  
    case TW_MT_SLA_ACK:  // slave receiver acked address
      TWDR = Message::head().reg;
      ack();
      break;
    case TW_MT_DATA_ACK: // slave receiver acked data
      if (Message::head().reading())
        start();
      else if (Message::head().any_data()) {
        TWDR = Message::head().get_data();
        ack();
      } else {
        stop();
        message_completed();
      }
      break;
  
    case TW_MR_DATA_ACK: // data received, ack sent
      Message::head().put_data(TWDR);
    case TW_MR_SLA_ACK:  // address sent, ack received
      Message::head().last_data() ? nack() : ack();
      break;
  
    case TW_MR_DATA_NACK: // data received, nack sent
      Message::head().put_data(TWDR);
      stop();
      message_completed();
      break;
      
    case TW_MR_SLA_NACK: // address sent, nack received
      stop();
      // App::app().serial.string("MR_SLA_NACK failure, trying again\r\n");
      // delay(500);
      start();
      break;
  
    case TW_MT_SLA_NACK:  // address sent, nack received
      stop();
      // App::app().serial.string("MT_SLA_NACK failure, trying again\r\n");
      // delay(500);
      start();
      break;
      
    case TW_MT_DATA_NACK: // data sent, nack received
      stop();
      // App::app().serial.string("MT_DATA_NACK failure, trying again\r\n");
      // delay(500);
      start();
      break;
  
    case TW_MT_ARB_LOST: // lost bus arbitration
      start();
      break;
  }
}

const void I2C::Task::message_completed() {
  Message::head().dequeue();
  if (Message::any()) start();
}

const void I2C::Task::new_message() {
  if (Message::just_one()) start();
}
    
void I2C::Message::enqueue() {
  index = 0;
  Queable<Message>::enqueue();
  App::app().i2c.task.new_message();
}

// TODO: need something here to prevent dequeing of messages if they are being transmitted!
