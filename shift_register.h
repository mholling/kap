#ifndef __SHIFT_REGISTER_H_
#define __SHIFT_REGISTER_H_

#include "spi.h"

class ShiftRegister {
public:
  ShiftRegister();
  void init();
  void set_bit(unsigned char n);
  void clear_bit(unsigned char n);
  
private:
  unsigned char data;
  
  class SendData : public Spi::WritePacket {
  private:
    const void toggle_select();
  public:
    SendData(const unsigned char& data) : Spi::WritePacket(&data, 1) { }
  } send_data;
};

#endif
