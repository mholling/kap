#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {
    
public:
  I2C();
  
  void init();
  const void interrupt();
  const void new_message(); // TODO: move into Message class as per SPI?
  
  class Message : public Queable<Message> {
  protected:
    enum read_write_value { read, write };
    void enqueue();
    
    inline static const void stop();
    inline static const void ack();
    inline static const void nack();
    inline static const void release();

    const void completed();

  private:
    volatile unsigned char * const buffer;
    const unsigned int length;
    const read_write_value read_write;
    unsigned int index;

  public:    
    Message(unsigned char address, unsigned char reg, volatile unsigned char *buffer, unsigned int length, read_write_value read_write) : Queable<Message>(), buffer(buffer), length(length), read_write(read_write), index(0), address(address), reg(reg) { }
    
    void interrupt();
    inline static const void start();
    
    inline void operator()() { enqueue(); }
        
    const unsigned char address;
    const unsigned char reg;
  };
  
  class ReadMessage : public Message {
  public:
    ReadMessage(unsigned char address, unsigned char reg, volatile unsigned char *buffer, int length) : Message(address, reg, buffer, length, read) { }
  };

  class WriteMessage : public Message {
  public:
    WriteMessage(unsigned char address, unsigned char reg, volatile unsigned char *buffer, int length) : Message(address, reg, buffer, length, write) { }
  };
};

#endif
