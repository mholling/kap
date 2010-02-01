#ifndef __ACCELEROMETER_H_
#define __ACCELEROMETER_H_

#include "i2c.h"
#include "scheduler.h"
#include <math.h>

class Accelerometer {
private:
  enum { i2c_address = 0x1d, thresh_tap_reg = 0x1d, ofsx_reg, ofsy_reg, ofsz_reg, dur_reg, latent_reg, window_reg, thresh_act_reg, thresh_inact_reg, time_inact_reg, time_inact_ctl_reg, act_inact_ctl_reg, thresh_ff_reg, time_ff_reg, tap_exes_reg, act_tap_status_reg, bw_rate_reg, power_ctl_reg, int_enable_reg, int_map_reg, int_source_reg, data_format_reg, datax0_reg, datax1_reg, datay0_reg, datay1_reg, dataz0_reg, dataz1_reg, fifo_ctl_reg, fifo_status_reg };
  
  class ModePacket : public I2C::WritePacket {
    volatile unsigned char data[1];
  public:
    enum mode_value { standby = 0x00, measure = 0x08 };
    ModePacket(mode_value mode) : I2C::WritePacket(data, i2c_address, power_ctl_reg, 1) { data[0] = mode; }
  };

  class RatePacket : public I2C::WritePacket {
    volatile unsigned char data[1];
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
    unsigned char data[8];
    inline int x() { return reinterpret_cast<int *>(data)[0]; }
    inline int y() { return reinterpret_cast<int *>(data)[1]; }
    inline int z() { return reinterpret_cast<int *>(data)[2]; }
  protected:
    void dequeue() {
      I2C::ReadPacket::dequeue();
      vector.x =  static_cast<float>(y());
      vector.y = -static_cast<float>(z());
      vector.z = -static_cast<float>(x());
    }
  public:
    MeasurementPacket() : I2C::ReadPacket(data, i2c_address, datax0_reg, 8) { }
    Vector vector;
    inline int ready() { return data[7] & 0x3f; }
    inline bool triggered() { return data[7] & 0x80; }
  };
  
  RatePacket set_rate;
  ModePacket standby, wake;
  InterruptConfigPacket configure_interrupt;
  DataFormatPacket set_data_format;
  
public:
  Accelerometer() : set_rate(RatePacket::hz_50), standby(ModePacket::standby), wake(ModePacket::measure) { }
  void init();

  MeasurementPacket measure;
  
  inline const Vector& vector() { return measure.vector; }
};

#endif
