#ifndef __GYROS_H_
#define __GYROS_H_

#include "analog.h"
#include "scheduler.h"
#include "angle.h"

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
  inline void init() { normal_mode(); enable(); }
  
  void disable();
  void enable();
  void normal_mode();
  void test_mode();
  
  void measure();
  
  class Gyro {
  private:
    const volatile Analog::Channel& value;
    const volatile Analog::Channel& reference;
    const float range;
    
    class Estimate : public Scheduler::Task {
    private:
      const Gyros::Gyro& gyro;
      const Angle& measured;

      float z1, z2;     // measured rate, angle
      float x1, x2, x3; // estimated angle, rate, bias
      const float dt;
      float p11, p12, p13, p21, p22, p23, p31, p32, p33;
      const float q1, q2, q3, q11, q12, q13, q21, q22, q23, q31, q32, q33;
      const float r11, r22;

      void predict();
      void correct();

    public:
      Estimate(const Gyro& gyro, const volatile Angle& measured);
      void run();

      float angle() const { return x1; }
      float rate() const { return x2; }
      float bias() const { return x3; }
    };
    
  public:
    Gyro(volatile Analog::Channel& value, volatile Analog::Channel& reference, unsigned int range, const volatile Angle& measured) : value(value), reference(reference), range(range), estimate(*this, measured) { }
    
    float operator ()() const;
    
    volatile Estimate estimate;
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
