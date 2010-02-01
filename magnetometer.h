#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include "scheduler.h"
#include "vector.h"
#include <math.h>

class Magnetometer {
private:
  enum { i2c_address = 0x1e, config_registers = 0x00, mode_register = 0x02, measurement_registers = 0x03 };

  class ConfigPacket : public I2C::WritePacket {
    volatile unsigned char data[2];
  public:
    ConfigPacket(unsigned char a = 0x10, unsigned char b = 0x20) : I2C::WritePacket(data, i2c_address, config_registers, 2) { data[0] = a; data[1] = b; }
  };
  
  class ModePacket : public I2C::WritePacket {
    volatile unsigned char data[1];
  public:
    enum mode_value { sleep = 0x03, idle = 0x02, single_shot = 0x01, continuous = 0x00 };
    ModePacket(mode_value mode) : I2C::WritePacket(data, i2c_address, mode_register, 1) { data[0] = mode; }
  };
    
  class MeasurementPacket : public I2C::ReadPacket {
  private:
    volatile unsigned char data[7];
    inline int x() { return static_cast<int>((static_cast<unsigned int>(data[0]) << 8) | static_cast<unsigned int>(data[1])); }
    inline int y() { return static_cast<int>((static_cast<unsigned int>(data[2]) << 8) | static_cast<unsigned int>(data[3])); }
    inline int z() { return static_cast<int>((static_cast<unsigned int>(data[4]) << 8) | static_cast<unsigned int>(data[5])); }
  protected:
    void dequeue() {
      I2C::ReadPacket::dequeue();
      vector.x =  static_cast<float>(y());
      vector.y = -static_cast<float>(z());
      vector.z = -static_cast<float>(x());
    }
  public:
    MeasurementPacket() : I2C::ReadPacket(data, i2c_address, measurement_registers, 7) { }
    Vector vector;
    inline bool ready() { return data[6] & 0x01; }
    inline char status() { return data[6]; }
  };
  
  ConfigPacket configure;
  ModePacket sleep, wake;
  
public:
  Magnetometer() : sleep(ModePacket::sleep), wake(ModePacket::continuous) { }
  void init() { configure(); wake(); }

  MeasurementPacket measure;
  
  inline const Vector& vector() { return measure.vector; }
};

#endif
