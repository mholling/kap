#ifndef __GYROS_H_
#define __GYROS_H_

#include "resource.h"
#include "app.h"
#include "analog.h"

class Gyros : private Resource {
private:
  enum { power_down_shift_register_bit = 6, self_test_shift_register_pin = 7 };
  class FakeChannel : public Analog::Channel {
  public:
    FakeChannel(unsigned int value) : Analog::Channel(0x0f) { data = value; }
  } fixed;

public:
  Gyros(App* app_) : Resource(app_), fixed(512), yaw(app().analog.yaw, fixed, 300), pitch(app().analog.pitch, app().analog.ref, 400), roll(app().analog.roll, app().analog.ref, 400) { }
  void init();
  
  void disable();
  void enable();
  void normal_mode();
  void test_mode();
  
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const unsigned int range;
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, unsigned int range) : value(value), reference(reference), range(range) { }
    float operator ()();
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
