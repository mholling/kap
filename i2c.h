#ifndef __I2C_H_
#define __I2C_H_

#include "queable.h"

class I2C {
    
public:
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

    virtual volatile unsigned char * const buffer() = 0; // TODO: should this just be a stored pointer instead?

  private:
    const unsigned int length;
    const read_write_value read_write;
    unsigned int index;
    const unsigned char address;
    const unsigned char reg;

  public:    
    Packet(unsigned char address, unsigned char reg, unsigned int length, read_write_value read_write) : length(length), read_write(read_write), index(0), address(address), reg(reg) { }
    
    void interrupt();
    void operator()();
    
    inline bool pending() { return Queable<Packet>::pending(); }
    inline static Packet& head() { return Queable<Packet>::head(); }
  };
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(unsigned char address, unsigned char reg, int length) : Packet(address, reg, length, read) { }
  };

  class WritePacket : public Packet {
  public:
    WritePacket(unsigned char address, unsigned char reg, int length) : Packet(address, reg, length, write) { }
  };
};

#endif
