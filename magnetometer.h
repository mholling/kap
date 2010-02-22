#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include "vector_packet.h"
#include "calibrate_task.h"
#include <math.h>

class Magnetometer {
private:
  enum { i2c_address = 0x1e, config_registers = 0x00, mode_register = 0x02, measurement_registers = 0x03, calibration_address = 0x100 };

  class ConfigPacket : public I2C::WritePacket {
    unsigned char data[2];
  public:
    ConfigPacket(unsigned char a = 0x10, unsigned char b = 0x20) : I2C::WritePacket(data, i2c_address, config_registers, 2) { data[0] = a; data[1] = b; }
  };
  
  class ModePacket : public I2C::WritePacket {
    unsigned char data[1];
  public:
    enum mode_value { sleep = 0x03, idle = 0x02, single_shot = 0x01, continuous = 0x00 };
    ModePacket(mode_value mode) : I2C::WritePacket(data, i2c_address, mode_register, 1) { data[0] = mode; }
  };
  
  class MeasurementPacket : public VectorPacket<MeasurementPacket> {
    friend class VectorPacket<MeasurementPacket>;
    
  protected:
    // // PCB:
    // int x() { return  static_cast<int>((static_cast<unsigned int>(data[2]) << 8) | static_cast<unsigned int>(data[3])); }
    // int y() { return -static_cast<int>((static_cast<unsigned int>(data[4]) << 8) | static_cast<unsigned int>(data[5])); }
    // int z() { return -static_cast<int>((static_cast<unsigned int>(data[0]) << 8) | static_cast<unsigned int>(data[1])); }

    // breadboard:
    int x() { return -static_cast<int>((static_cast<unsigned int>(data[2]) << 8) | static_cast<unsigned int>(data[3])); }
    int y() { return -static_cast<int>((static_cast<unsigned int>(data[4]) << 8) | static_cast<unsigned int>(data[5])); }
    int z() { return  static_cast<int>((static_cast<unsigned int>(data[0]) << 8) | static_cast<unsigned int>(data[1])); }
    
  public:
    MeasurementPacket(unsigned char i2c_address, unsigned char i2c_registers) : Base(i2c_address, i2c_registers) { }
  };
  
  ConfigPacket configure;
  ModePacket sleep;
  ModePacket wake;

public:
  Magnetometer() : sleep(ModePacket::sleep), wake(ModePacket::continuous), measure(i2c_address, measurement_registers), calibrate(measure, calibration_address, 0.97) { }
  void init() { configure(); wake(); calibrate.init(); }
  
  MeasurementPacket measure;
  CalibrateTask calibrate;
};

#endif
