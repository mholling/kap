#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include <math.h>

class Magnetometer {
private:
  enum { i2c_address = 0x1e };

  class Config : public I2C::Message {
  public:
    volatile unsigned char value[2];
    Config() : I2C::Message(i2c_address, 0x00, value, 2) { }
    void set_defaults() { value[0] = 0x10; value[1] = 0x20; write(); }
  };
  
  class Mode : public I2C::Message {
  public:
    volatile unsigned char value;
    Mode() : I2C::Message(i2c_address, 0x02, &value, 1) { }
    void set_continuous() { value = 0x00; write(); }
    void set_single() { value = 0x01; write(); }
    void set_idle() { value = 0x02; write(); }
    void set_sleep() { value = 0x03; write(); }
  };
  
  class Status : public I2C::ReadMessage {
  public:
    volatile unsigned char value;
    Status() : I2C::ReadMessage(i2c_address, 0x09, &value, 1) { }
    const bool ready() { return value & 0x01; }
  };
  
  class Vector : public I2C::ReadMessage {
    volatile unsigned char value[6];
  public:
    Vector() : I2C::ReadMessage(i2c_address, 0x03, value, 6) { }
    int x() { int i = (value[0] << 8) + value[1]; return i; }
    int y() { int i = (value[2] << 8) + value[3]; return i; }
    int z() { int i = (value[4] << 8) + value[5]; return i; }
    int bearing() { return static_cast<int>(atan2(x(), y()) * 180.0 / M_PI); }
  };
  
  Config config;
  Mode mode;
  Status status;
  Vector vector;
  
public:
  void configure() { config.set_defaults(); }
  void sleep() { mode.set_sleep(); }
  void wake() { mode.set_continuous(); }

  void show_bearing();
  void show_status();
};

#endif
