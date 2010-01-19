#ifndef __GYROS_H_
#define __GYROS_H_

#include "resource.h"
#include "app.h"

class Gyros : private Resource {
private:
  enum { power_down_shift_register_bit = 6, self_test_shift_register_pin = 7 };

public:
  Gyros(App* app) : Resource(app), yaw(0, 4, 600), pitch(1, 3, 800), roll(2, 3, 800) { }
  void init();
  
  void disable();
  void enable();
  void normal_mode();
  void test_mode();
  
  class Gyro {
  private:
    const unsigned int value_channel;
    const unsigned int reference_channel;
    const unsigned int scale;
    
  public:
    Gyro(unsigned int value_channel, unsigned int origin_channel, unsigned int scale) : value_channel(value_channel), origin_channel(origin_channel), scale(scale) { }
    float operator ()();
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
