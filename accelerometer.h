#ifndef __ACCELEROMETER_H_
#define __ACCELEROMETER_H_

#include "i2c.h"
#include "vector.h"

class Accelerometer {
private:
  enum { i2c_address = 0x53, bw_rate_reg = 0x2c, power_ctl_reg = 0x2d, int_enable_reg = 0x2e, data_format_reg = 0x31, datax0_reg = 0x32 };
  
  class ModePacket : public I2C::WritePacket {
    unsigned char data[1];
  public:
    enum mode_value { standby = 0x00, measure = 0x08 };
    ModePacket(mode_value mode) : I2C::WritePacket(data, i2c_address, power_ctl_reg, 1) { data[0] = mode; }
  };

  class RatePacket : public I2C::WritePacket {
    unsigned char data[1];
  public:
    enum rate_value { hz12 = 0x07, hz_25, hz_50, hz_100, hz_200, hz_400, hz_800 };
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
  protected:
    unsigned char data[6];

  public:
    MeasurementPacket(unsigned char i2c_address, unsigned char i2c_registers) : I2C::ReadPacket(data, i2c_address, i2c_registers, 6) { }
    
    Vector vector;
    virtual void before_dequeue() {
      vector[0] =  reinterpret_cast<int *>(data)[0];
      vector[1] = -reinterpret_cast<int *>(data)[1];
      vector[2] = -reinterpret_cast<int *>(data)[2];
    }
  };
  
  RatePacket set_rate;
  ModePacket standby;
  ModePacket wake;
  InterruptConfigPacket configure_interrupt;
  DataFormatPacket set_data_format;

public:
  Accelerometer() : set_rate(RatePacket::hz_100), standby(ModePacket::standby), wake(ModePacket::measure), measure(i2c_address, datax0_reg) { }
  inline void init() { set_rate(); set_data_format(); configure_interrupt(); wake(); }
  
  MeasurementPacket measure;
  
  inline Vector vector() const { return measure.vector; }
};

#endif
