#ifndef __GYROS_H_
#define __GYROS_H_

class App;
extern App app;

#include "analog.h"
#include "scheduler.h"

class Gyros {
private:
  enum { power_down_shift_register_bit = 0, self_test_shift_register_pin = 5 };
  class FakeChannel : public Analog::Channel {
  public:
    FakeChannel(float value) : Analog::Channel(0x0f) { data = 1024 * value; }
  } fixed;
  
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const float range;
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, unsigned int range) : value(value), reference(reference), range(range) { }
    float operator ()() const;
    bool pending() { CriticalSection cs; return value.pending() || reference.pending(); }
    void wait() { value.wait(); reference.wait(); }
  };

public:
  Gyros();
  void init();
  
  void disable();
  void enable();
  void normal_mode();
  void test_mode();
  
  void measure();
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
