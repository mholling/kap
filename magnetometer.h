#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include <math.h>

class Magnetometer {
private:
  enum { i2c_address = 0x1e, config_registers = 0x00, mode_register = 0x02, vector_registers = 0x03, status_register = 0x09 };

  class ConfigMessage : public I2C::WriteMessage {
    volatile unsigned char data[2];
  public:
    ConfigMessage(unsigned char a = 0x10, unsigned char b = 0x20) : I2C::WriteMessage(i2c_address, config_registers, data, 2) { data[0] = a; data[1] = b; }
  };
  
  class ModeMessage : public I2C::WriteMessage {
    volatile unsigned char data[1];
  public:
    enum mode_value { sleep = 0x03, idle = 0x02, single_shot = 0x01, continuous = 0x00 };
    ModeMessage(mode_value mode) : I2C::WriteMessage(i2c_address, mode_register, data, 1) { data[0] = mode; }
  };
  
  class StatusMessage : public I2C::ReadMessage {
    volatile unsigned char data[1];
  public:
    StatusMessage() : I2C::ReadMessage(i2c_address, status_register, data, 1) { }
    inline const bool ready() { return data[0] & 0x01; }
    inline const char value() { return data[0]; }
  };
  
  class VectorMessage : public I2C::ReadMessage {
    volatile unsigned char data[6];
  public:
    VectorMessage() : I2C::ReadMessage(i2c_address, vector_registers, data, 6) { }
    inline int x() { int i = (data[0] << 8) + data[1]; return i; }
    inline int y() { int i = (data[2] << 8) + data[3]; return i; }
    inline int z() { int i = (data[4] << 8) + data[5]; return i; }
    inline int bearing() { return static_cast<int>(atan2(x(), y()) * 180.0 / M_PI); }
  };
  
  ConfigMessage configure;
  ModeMessage sleep, wake;
  StatusMessage status;
  VectorMessage vector;
  
public:
  Magnetometer() : sleep(ModeMessage(ModeMessage::sleep)), wake(ModeMessage(ModeMessage::continuous)) { }
  
  void init() { configure(); wake(); }

  void show_bearing();
  void show_status();
};

#endif
