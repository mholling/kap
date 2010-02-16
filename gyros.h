#ifndef __GYROS_H_
#define __GYROS_H_

#include "analog.h"
#include "scheduler.h"

class Gyros {
private:
  enum { power_down_shift_register_bit = 0, self_test_shift_register_pin = 5 };
  
  Analog::Channel yaw_channel;
  Analog::Channel pitch_channel;
  Analog::Channel roll_channel;
  Analog::Channel ref_channel;

  class FakeChannel : public Analog::Channel {
  public:
    FakeChannel(float value) : Analog::Channel(0x0f) { data = 1024 * value; }
  } fixed_channel;

public:
  Gyros();
  inline void init() volatile { normal_mode(); enable(); }
  
  void disable() volatile;
  void enable() volatile;
  void normal_mode() volatile;
  void test_mode() volatile;
  
  void measure();
  
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const float range;
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, unsigned int range) : value(value), reference(reference), range(range) { }
    float operator ()() const volatile;
    float operator ()() const;
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
