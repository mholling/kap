#ifndef __GYROS_H_
#define __GYROS_H_

#include "analog.h"
#include "shift_register.h"
#include "scheduler.h"
#include "angle.h"

class Gyros {
private:
  enum { power_down_shift_register_pin = 0, self_test_shift_register_pin = 5 };
  
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
  void init() { normal_mode(true); enable(true); }
  
   ShiftRegister::Bit disable;
   ShiftRegister::Bit enable;
   ShiftRegister::Bit test_mode;
   ShiftRegister::Bit normal_mode;
  
  void measure();
  
  class Gyro {
  private:
    const Analog::Channel& value;
    const Analog::Channel& reference;
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
      Estimate(const Gyro& gyro, const Angle& measured);
      void run();

      float angle() const { return x1; }
      float rate() const { return x2; }
      float bias() const { return x3; }
    };
    
  public:
    Gyro(Analog::Channel& value,  Analog::Channel& reference, unsigned int range, const Angle& measured) : value(value), reference(reference), range(range), estimate(*this, measured) { }
    
    float operator ()() const;
    
     Estimate estimate;
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
