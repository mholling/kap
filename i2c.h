#ifndef __I2C_H_
#define __I2C_H_

// TODO: reviews inlines everywhere

#include "interrupt_driven.h"

class I2C : public InterruptDriven<I2C> {  
public:
  enum { frequency = 300000 }; // cannot use 400000 as TWSTO doesn't get released and a hang results
  
  I2C();
  
  class Packet : public Item {
  protected:
    enum read_write_value { read, write };
    
    void initiate();
    bool process();
    void terminate();

  private:
    static const void start();
    static const void stop();
    static const void ack();
    static const void nack();
    static const void release();
    
    unsigned char * const buffer;
    const unsigned int length;
    const read_write_value read_write;
    unsigned int index;
    const unsigned char address;
    const unsigned char reg;

  public:    
    Packet(unsigned char * buffer, unsigned char address, unsigned char reg, unsigned int length, read_write_value read_write) : buffer(buffer), length(length), read_write(read_write), index(0), address(address), reg(reg) { }
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(unsigned char * buffer, unsigned char address, unsigned char reg, int length) : Packet(buffer, address, reg, length, read) { }
  };

  class WritePacket : public Packet {
  public:
    WritePacket(unsigned char * buffer, unsigned char address, unsigned char reg, int length) : Packet(buffer, address, reg, length, write) { }
  };
};

#endif
