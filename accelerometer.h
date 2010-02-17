#ifndef __ACCELEROMETER_H_
#define __ACCELEROMETER_H_

#include "i2c.h"
#include "scheduler.h"
#include "vector.h"
#include <math.h>

// TODO: add a calibration task for the accelerometer as well?

class Accelerometer {
private:
  enum { i2c_address = 0x1d, bw_rate_reg = 0x2c, power_ctl_reg = 0x2d, int_enable_reg = 0x2e, data_format_reg = 0x31, datax0_reg = 0x32 };
  
  class ModePacket : public I2C::WritePacket {
    unsigned char data[1];
  public:
    enum mode_value { standby = 0x00, measure = 0x08 };
    ModePacket(mode_value mode) : I2C::WritePacket(data, i2c_address, power_ctl_reg, 1) { data[0] = mode; }
  };

  class RatePacket : public I2C::WritePacket {
    unsigned char data[1];
  public:
    enum rate_value { hz12 = 0x07, hz_25, hz_50, hz_100, hz_200, hz_400,hz_800 };
    RatePacket(rate_value rate) : I2C::WritePacket(data, i2c_address, bw_rate_reg, 1) { data[0] = rate; }
  };

  class InterruptConfigPacket : public I2C::WritePacket {
    static unsigned char data[2];
  public:
    InterruptConfigPacket() : I2C::WritePacket(data, i2c_address, int_enable_reg, 2) { }
  };

  class DataFormatPacket : public I2C::WritePacket {
    static unsigned char data[1];
  public:
    DataFormatPacket() : I2C::WritePacket(data, i2c_address, data_format_reg, 1) { }
  };
    
  class ConfigPacket : public I2C::WritePacket {
    static unsigned char data[6];
  public:
    ConfigPacket() : I2C::WritePacket(data, i2c_address, bw_rate_reg, 6) { }
  };
  
  class MeasurementPacket : public I2C::ReadPacket {
  private:
    unsigned char data[6];
    inline int x() { return reinterpret_cast<int *>(data)[0]; }
    inline int y() { return reinterpret_cast<int *>(data)[1]; }
    inline int z() { return reinterpret_cast<int *>(data)[2]; }
  protected:
    void terminate() {
      I2C::ReadPacket::terminate();
      
      // // for PCB:
      // vector.x =  static_cast<float>(y());
      // vector.y = -static_cast<float>(z());
      // vector.z = -static_cast<float>(x());
      
      // for breadboard:
      vector[0] = -static_cast<float>(y());
      vector[1] = -static_cast<float>(z());
      vector[2] =  static_cast<float>(x());
    }
  public:
    MeasurementPacket() : I2C::ReadPacket(data, i2c_address, datax0_reg, 6) { }
    Vector vector;
  };
  
  volatile RatePacket set_rate;
  volatile ModePacket standby;
  volatile ModePacket wake;
  volatile InterruptConfigPacket configure_interrupt;
  volatile DataFormatPacket set_data_format;

public:
  Accelerometer() : set_rate(RatePacket::hz_50), standby(ModePacket::standby), wake(ModePacket::measure) { }
  void init() { set_rate(); set_data_format(); configure_interrupt(); wake(); }
  
  volatile MeasurementPacket measure;
};

#endif
