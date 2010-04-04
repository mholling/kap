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
    Channels() : yaw(0), pitch(3), roll(2), ref(1), fixed(0.5) { }
    
    Analog::Channel yaw;
    Analog::Channel pitch;
    Analog::Channel roll;
    Analog::Channel ref;
    FakeChannel fixed;
  } channels;
  
  ShiftRegister::Bit disable;
  ShiftRegister::Bit enable;
  ShiftRegister::Bit test_mode;
  ShiftRegister::Bit normal_mode;
    
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const float range;
    
    class Estimate : public Timer::Task {
    private:
      const Gyros::Gyro& gyro;
      const Attitude::Measure::angle_method_type angle_method;
      
      Matrix<2, 1> x; // angle, bias
      Matrix<2, 2> P; // error covariance
      Matrix<2, 2> Q; // process covariance
      Matrix<1, 1> R; // variance of angle measurement noise
      Matrix<2, 2> A; // state transform matrix
      Matrix<2, 2> At;
      Matrix<2, 1> B; // input driving matrix
      Matrix<1, 1> u; // driving input (biased gyro rate)
      Matrix<1, 2> H; // prediction matrix
      Matrix<2, 1> Ht;
      Matrix<1, 1> z; // measured value (angle measurement)
      
      // TODO: optimise code to get rid of matrix calculations once tuned.

      void predict();
      void correct();

    public:
      Estimate(const Gyro& gyro, Attitude::Measure::angle_method_type angle_method, float angle_sd, float rate_sd, float bias_sd);
      void run();

      float angle() const { return x(0,0); }
      float  bias() const { return x(1,0); }
    };
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, float range, Attitude::Measure::angle_method_type angle_method, float angle_sd, float rate_sd, float bias_sd);
        
    float rate() const;
    Estimate estimate;
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
};

#endif
