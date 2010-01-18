#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include <math.h>

class Magnetometer {
private:
  enum { i2c_address = 0x1e, config_registers = 0x00, mode_register = 0x02, vector_status_registers = 0x03 };

  class ConfigPacket : public I2C::WritePacket {
    volatile unsigned char data[2];
    inline volatile unsigned char * const buffer() { return data; };
  public:
    ConfigPacket(unsigned char a = 0x10, unsigned char b = 0x20) : I2C::WritePacket(i2c_address, config_registers, 2) { data[0] = a; data[1] = b; }
  };
  
  class ModePacket : public I2C::WritePacket {
    volatile unsigned char data[1];
    inline volatile unsigned char * const buffer() { return data; };
  public:
    enum mode_value { sleep = 0x03, idle = 0x02, single_shot = 0x01, continuous = 0x00 };
    ModePacket(mode_value mode) : I2C::WritePacket(i2c_address, mode_register, 1) { data[0] = mode; }
  };
    
  class VectorStatusPacket : public I2C::ReadPacket {
    volatile unsigned char data[7];
    inline volatile unsigned char * const buffer() { return data; };
  public:
    VectorStatusPacket() : I2C::ReadPacket(i2c_address, vector_status_registers, 7) { }
    inline int x() { int i = (data[0] << 8) + data[1]; return i; }
    inline int y() { int i = (data[2] << 8) + data[3]; return i; }
    inline int z() { int i = (data[4] << 8) + data[5]; return i; }
    inline int bearing() { return static_cast<int>(atan2(x(), y()) * 180.0 / M_PI); }
    inline const bool ready() { return data[6] & 0x01; }
    inline const char status() { return data[6]; }
  };
  
  ConfigPacket configure;
  ModePacket sleep, wake;
  VectorStatusPacket vector_status;
  
public:
  Magnetometer() : sleep(ModePacket::sleep), wake(ModePacket::continuous) { }
  
  void init() { configure(); wake(); }

  void show_bearing();
};

#endif
