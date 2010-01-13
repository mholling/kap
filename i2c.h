#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {
protected:
  const void start();
  const void stop();
  const void ack();
  const void nack();
  const void release();
  
  const void next_message();
  
public:
  I2C();
  
  const void interrupt();
  const void new_message();
  
  class Message : public Queable<Message> {
  public:
    enum mode_value { read_mode, write_mode };
    
    Message() : address(0), reg(0), mode(read_mode), data(0), length(0), index(0) { }
    Message(unsigned char address, unsigned char reg, volatile unsigned char * data, int length) : address(address), reg(reg), mode(read_mode), data(data), length(length), index(0) { }
    
    bool reading() { return mode == read_mode; }
    bool writing() { return mode == write_mode; }
    
    void read() { mode = read_mode; enqueue(); }
    void write() { mode = write_mode; enqueue(); }
    
    const unsigned char address;
    const unsigned char reg;

    void put_data(unsigned char b) { data[index++] = b; }
    unsigned char get_data() { return data[index++]; }
    bool any_data() { return index < length; }
    bool last_data() { return index == length - 1; }

  protected:
    void enqueue();
    
    mode_value mode;
    volatile unsigned char * const data;
    const int length;
    int index;
  };
  
  class ReadMessage : public Message {
  private:
    void write() {};

  public:
    ReadMessage(unsigned char address, unsigned char reg, volatile unsigned char *data, int length) : Message(address, reg, data, length) { }
  };
};

#endif
