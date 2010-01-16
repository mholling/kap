#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {
protected:
  inline const void start();
  inline const void stop();
  inline const void ack();
  inline const void nack();
  inline const void release();
  
  const void message_completed();
    
public:
  I2C();
  
  const void init();
  const void interrupt();
  const void new_message();
  
  class Message : public Queable<Message> {
  private:
    Message& operator=(const Message&);

  public:
    enum read_write_value { read, write };
    
    Message(unsigned char address, unsigned char reg, volatile unsigned char *buffer, int length, read_write_value read_write) : Queable<Message>(), address(address), reg(reg), buffer(buffer), length(length), read_write(read_write), index(0) { }
        
    inline void operator()() { enqueue(); }
    
    inline const bool reading() { return read_write == read; }
    
    const unsigned char address;
    const unsigned char reg;

    inline void put_data(unsigned char b) { buffer[index++] = b; } // TODO: should these be protected/private?
    inline unsigned char get_data() { return buffer[index++]; }
    inline bool any_data() { return index < length; }
    inline bool last_data() { return index == length - 1; }

  protected:
    void enqueue();
    
  private:
    volatile unsigned char * const buffer;
    const int length;
    const read_write_value read_write;

    int index;
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
