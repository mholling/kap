#ifndef __GYROS_H_
#define __GYROS_H_

#include "analog.h"
#include "shift_register.h"
#include "timer.h"
#include "attitude.h"
#include "matrix.h"

class Gyros {
private:
  enum { power_down_shift_register_pin = 0, self_test_shift_register_pin = 5 };

  class FakeChannel : public Analog::Channel {
  public:
    FakeChannel(float value) : Analog::Channel(0x0f) { data = 1024 * value; }
  };
  
public:
  Gyros();
  void init() { normal_mode(true); enable(true); }
  
  class Channels {
  public:
    Channels() : x(3), y(2), z(0), ref(1), fixed(0.5) { }
    
    Analog::Channel x, y, z, ref;
    FakeChannel fixed;
  } channels;
  
  ShiftRegister::Bit disable, enable, test_mode, normal_mode;
    
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const float range;
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, float range);
        
    float rate() const;
  };
  
  Gyro x, y, z; // TODO: make these private
  
  Vector rates() const;
};

#endif
