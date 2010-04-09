#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "interrupt_driven.h"

class Eeprom : public InterruptDriven {
protected:
  class Packet : public Driven<Packet> {
    friend class Driven<Packet>;
    friend class Eeprom;
    
    // unsigned int fletcher8();
    unsigned int crc16();
    void validate();
    bool valid();
    
  public:
    enum operation_value { reading, writing };

    Packet(unsigned int address, char * buffer, unsigned int length, operation_value operation) : address(address), buffer(buffer), length(length), operation(operation), index(0) { }
    
    template <typename T>
    Packet& operator <<(const T& t) {
      const char * c = reinterpret_cast<const char *>(const_cast<const T*>(&t));
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        buffer[index++] = *c;
      return *this;
    }

    template <typename T>
    Packet& operator >>(T& t) {
      char * c = reinterpret_cast<char *>(const_cast<T*>(&t));
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        *c = buffer[index++];
      return *this;
    }

  protected:
    void initiate();
    bool process();
    void terminate();
    
  private:
    const unsigned int address;
    char * const buffer;
    const unsigned int length;
    const operation_value operation;
    int unsigned index;
    unsigned int checksum;
  };
  
public:
  Eeprom() { }
  
  class ReadPacket : public Packet {
  protected:
    virtual void before_dequeue();
    bool valid;
    
  public:
    ReadPacket(unsigned int address, char * buffer, unsigned int length) : Packet(address, buffer, length, reading), valid(false) { }
    bool operator ()() { return Packet::operator()(true) && valid; }
  };
  
  class WritePacket : public Packet {
  protected:
    virtual void after_enqueue();

  public:
    WritePacket(unsigned int address, char * buffer, unsigned int length) : Packet(address, buffer, length, writing) { }
  };
  
  void interrupt() { Packet::head().interrupt(); }
};

#endif
