#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "interrupt_driven.h"
#include "safe.h"

class Eeprom : public InterruptDriven {
protected:
  class Packet : public Item {
  public:
    enum operation_value { reading, writing };

    Packet(unsigned int address, char * buffer, unsigned int length, operation_value operation) : address(address), buffer(buffer), length(length), operation(operation), index(0), checksum(0) { }
        
    inline bool valid() volatile { return Safe<Packet>(this)->valid(); }
    bool valid();
        
    template <typename T>
    inline Packet& operator <<(const volatile T& t) volatile { return *Safe<Packet>(this) << t; }

    template <typename T>
    inline Packet& operator >>(volatile T& t) volatile { return *Safe<Packet>(this) >> t; }

    inline bool operator ()(bool block = false) volatile { return enqueue(block); }
    
  protected:
    void initiate();
    bool process();
    void terminate();
    
  private:
    template <typename T>
    Packet& operator <<(const volatile T& t) { // TODO: disallow if pending?
      const char * c = reinterpret_cast<const char *>(const_cast<const T*>(&t));
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        checksum ^= (buffer[index++] = *c);
      return *this;
    }
    
    template <typename T>
    Packet& operator >>(volatile T& t) { // TODO: disallow if pending?
      char * c = reinterpret_cast<char *>(const_cast<T*>(&t));
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        *c = buffer[index++];
      return *this;
    }

    const unsigned int address;
    char * const buffer;
    const unsigned int length;
    const operation_value operation;
    int unsigned index;
    char checksum;
  };
  
public:
  Eeprom() { }
  
  class ReadPacket : public Packet {
  public:
    ReadPacket(unsigned int address, char * buffer, unsigned int length) : Packet(address, buffer, length, reading) { }
    inline bool operator ()() volatile { return Packet::operator()(true) && valid(); }
  };
  
  class WritePacket : public Packet {
  public:
    WritePacket(unsigned int address, char * buffer, unsigned int length) : Packet(address, buffer, length, writing) { }
  };
};

#endif
