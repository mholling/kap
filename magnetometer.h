#ifndef __MAGNETOMETER_H_
#define __MAGNETOMETER_H_

#include "i2c.h"
#include "timer.h"
#include "vector.h"
#include "persistent.h"

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
  
  class MeasurementPacket : public I2C::ReadPacket {
  protected:
    unsigned char data[6];

  public:
    MeasurementPacket(unsigned char i2c_address, unsigned char i2c_registers) : I2C::ReadPacket(data, i2c_address, i2c_registers, 6) { }
    
    Vector vector;
    virtual void before_dequeue() {
      vector[0] =  static_cast<int>((static_cast<unsigned int>(data[0]) << 8) | static_cast<unsigned int>(data[1]));
      vector[1] = -static_cast<int>((static_cast<unsigned int>(data[2]) << 8) | static_cast<unsigned int>(data[3]));
      vector[2] = -static_cast<int>((static_cast<unsigned int>(data[4]) << 8) | static_cast<unsigned int>(data[5]));
    }
  };

  class Calibrate : public Timer::Task {
  protected:
    enum { autosave_minutes = 5 };

    class State {
    public:
      float P0[1], P1[2], P2[3], P3[4];
      float w3;
      float h2, b2;
      Vector bias;

      void defaults();
    };
    
    Persistent<State> state;
    Vector previous;
    const float lambda;

  public:
    Calibrate(unsigned int address, float lambda) : state(address), lambda(lambda) { }

    inline void init() { state.init(); }
    void run();

    inline Vector bias() const { return state.bias; }
  };
  
  ConfigPacket configure;
  ModePacket sleep;
  ModePacket wake;

public:
  Magnetometer() : sleep(ModePacket::sleep), wake(ModePacket::continuous), measure(i2c_address, measurement_registers), calibrate(calibration_address, 0.97) { }
  inline void init() { configure(); wake(); calibrate.init(); }
  
  MeasurementPacket measure;
  Calibrate calibrate;
  
  Vector vector() const;
};

#endif
