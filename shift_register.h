#ifndef __SHIFT_REGISTER_H_
#define __SHIFT_REGISTER_H_

#include "spi.h"
#include "safe.h"

class ShiftRegister {
public:
  ShiftRegister();
  void init() volatile { send_data(); };
  
  void set_bit(unsigned char n) volatile { Safe<ShiftRegister>(this)->set_bit(n); }
  void clear_bit(unsigned char n) volatile { Safe<ShiftRegister>(this)->clear_bit(n); }

  void set_bit(unsigned char n);
  void clear_bit(unsigned char n);
  
private:
  unsigned char data;
  
  class SendData : public Spi::WritePacket {
  private:
    void toggle_select();
  public:
    SendData(const unsigned char& data) : Spi::WritePacket(&data, 1) { }
  };
  
  SendData send_data;
};

#endif
