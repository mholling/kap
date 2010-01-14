#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"
#include "scheduler.h"

class I2C {
public:
  I2C();
  
  const void init();
  const void interrupt();
  
  class Message : public Queable<Message> {
  private:
    Message& operator=(const Message&);

  public:
    enum read_write_value { read, write };
    
    Message(unsigned char address, unsigned char reg, volatile unsigned char *buffer, int length, read_write_value read_write) : Queable<Message>(), address(address), reg(reg), buffer(buffer), length(length), read_write(read_write), index(0) { }
        
    void operator()() { enqueue(); }
    
    const bool reading() { return read_write == read; }
    
    const unsigned char address;
    const unsigned char reg;

    void put_data(unsigned char b) { buffer[index++] = b; } // TODO: should these be protected/private?
    unsigned char get_data() { return buffer[index++]; }
    bool any_data() { return index < length; }
    bool last_data() { return index == length - 1; }

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
  
  class Task : public Scheduler::Task {
  protected:
    const void start();
    const void stop();
    const void ack();
    const void nack();
    const void release();
    
    const void message_completed();
      
  public:
    const void new_message();
 
    Task() : Scheduler::Task(10) { }
    void operator()();
  } task;
};

#endif
