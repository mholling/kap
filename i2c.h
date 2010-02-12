#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {  
public:
  enum { frequency = 300000 }; // cannot use 400000 as TWSTO doesn't get released and a hang results
  
  I2C();
  inline void init() volatile { }
  
  class Packet : public Queable<Packet> {
  protected:
    enum read_write_value { read, write };
    inline void dequeue() volatile { Safe<Packet>(this)().dequeue(); }
    void dequeue();
    inline static const void start();
    inline static const void stop();
    inline static const void ack();
    inline static const void nack();
    inline static const void release();

  private:
    unsigned char * const buffer;
    const unsigned int length;
    const read_write_value read_write;
    unsigned int index;
    const unsigned char address;
    const unsigned char reg;

  public:    
    Packet(unsigned char * buffer, unsigned char address, unsigned char reg, unsigned int length, read_write_value read_write) : buffer(buffer), length(length), read_write(read_write), index(0), address(address), reg(reg) { }
    
    void interrupt();
    void operator ()(bool block = false) volatile;
    void operator ()();
    
    inline void wait() volatile { do { } while (next != 0); }
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
