#ifndef __SHIFT_REGISTER_H_
#define __SHIFT_REGISTER_H_

#include "spi.h"

class ShiftRegister {
public:
  ShiftRegister();
  void init() ;
  
  unsigned char bits;
  
  class Bit : public Spi::WritePacket {
  private:
    unsigned char data;
    const unsigned char mask;
    const bool set;
    
  protected:
    void after_enqueue();
    
  public:
    Bit(unsigned int bit, bool set) : Spi::WritePacket(PORTD, PIND4, &data, 1), mask(1 << bit), set(set) { }
  };
};

#endif
