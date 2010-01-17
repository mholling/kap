#ifndef __SHIFT_REGISTER_H_
#define __SHIFT_REGISTER_H_

#include "spi.h"

class ShiftRegister {
public:
  ShiftRegister();
  void init();
  
  class SetValue : public Spi::Packet {
  private:
    unsigned char data;
    const void toggle_select();
  public:
    SetValue() : Spi::Packet(&data, 0, 1, 0) { }
    void operator ()(unsigned char value);
  } set_value;
};

#endif
