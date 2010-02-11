#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {  
public:
  enum { frequency = 300000 }; // cannot use 400000 as TWSTO doesn't get released and a hang results
  
  I2C();
  inline void init() { }
  
  class Packet : protected Queable<Packet> {
  protected:
    enum read_write_value { read, write };
    void dequeue();
    inline static const void start();
    inline static const void stop();
    inline static const void ack();
    inline static const void nack();
    inline static const void release();

  private:
    volatile unsigned char * const buffer;
    const unsigned int length;
    const read_write_value read_write;
    unsigned int index;
    const unsigned char address;
    const unsigned char reg;

  public:    
    Packet(volatile unsigned char * buffer, unsigned char address, unsigned char reg, unsigned int length, read_write_value read_write) : buffer(buffer), length(length), read_write(read_write), index(0), address(address), reg(reg) { }
    
    void interrupt();
    void operator ()(bool block = false);
    
    inline bool pending() { return Queable<Packet>::pending(); }
    inline void wait() { Queable<Packet>::wait(); }
    inline static Packet& head() { return Queable<Packet>::head(); }
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(volatile unsigned char * buffer, unsigned char address, unsigned char reg, int length) : Packet(buffer, address, reg, length, read) { }
  };

  class WritePacket : public Packet {
  public:
    WritePacket(volatile unsigned char * buffer, unsigned char address, unsigned char reg, int length) : Packet(buffer, address, reg, length, write) { }
  };
};

#endif
