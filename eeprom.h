#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "interrupt_driven.h"

class Eeprom : public InterruptDriven {
public:
  Eeprom() { }
  
  class Packet : public Item {
  public:
    enum operation_value { reading, writing };

    Packet(unsigned int address, char * buffer, unsigned int length) : address(address), buffer(buffer), length(length), operation(reading), index(0) { }
    
    inline void write(bool block = false) volatile { (*this)(writing, block); }
    inline void  read(bool block = false) volatile { (*this)(reading, block); }
        
    template <typename T>
    inline Packet& operator <<(const T& t) volatile { return Safe<Packet>(this)() << t; }
      
    template <typename T>
    Packet& operator <<(const T& t) { // TODO: disallow if pending?
      const char * c = reinterpret_cast<const char *>(&t);
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        buffer[index++] = *c;
      return *this;
    }

    template <typename T>
    inline Packet& operator >>(T& t) volatile { return Safe<Packet>(this)() >> t; }
    
    template <typename T>
    Packet& operator >>(T& t) { // TODO: disallow if pending?
      char * c = reinterpret_cast<char *>(&t);
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
    operation_value operation;
    int unsigned index;

    void operator ()(operation_value op, bool block) volatile;
  };
};

#endif
