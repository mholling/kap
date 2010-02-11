#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "queable.h"

class Eeprom {
public:
  Eeprom() { }
  inline void init() { }
  
  class Packet : public Queable<Packet> {
  public:
    enum operation_value { reading, writing };

    Packet(unsigned int address, char * buffer, unsigned int length) : address(address), buffer(buffer), length(length), operation(reading), index(0) { }
    
    void interrupt();
    
    inline void write(bool block = false) { (*this)(writing, block); }
    inline void  read(bool block = false) { (*this)(reading, block); }
    
    template <typename T>
    Packet& operator <<(const T& t) { // TODO: disallow if pending?
      const char * c = reinterpret_cast<const char *>(&t);
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        buffer[index++] = *c;
      return *this;
    }

    template <typename T>
    Packet& operator >>(T& t) { // TODO: disallow if pending?
      char * c = reinterpret_cast<char *>(&t);
      for (unsigned int n = 0; n < sizeof(T); n++, c++)
        *c = buffer[index++];
      return *this;
    }

  protected:
    void dequeue();
    void start();
    void stop();
    
  private:
    const unsigned int address;
    char * const buffer;
    const unsigned int length;
    operation_value operation;
    int unsigned index;

    void operator ()(operation_value op, bool block);
  };
};

#endif
