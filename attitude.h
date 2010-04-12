#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_

#include "timer.h"
#include "quaternion.h"

class Attitude {
public:
  class Measure : public Timer::Task {
  private:
    const float a2; // a1 = 1.0, a2 is relative to a1
    
    Quaternion quaternion;
    
  public:
    Measure() : a2(0.5) { }
    void run();
    
    float   yaw() const { return quaternion.yaw(); }
    float pitch() const { return quaternion.pitch(); }
    float  roll() const { return quaternion.roll(); }
  };
  
  class Estimate : public Timer::Task {
  public:
    class Kalman {
    private:
      float x1, x2;
      float P11, P12, P22;
      float Q11, Q12, Q22;
      float R;

      float old_angle;
      int revolutions;
      float actual_rate;

    public:
      Kalman() : old_angle(0.0), revolutions(0) { }
      
      void init(float initial_angle, float initial_rate, float angle_variance, float rate_variance, float bias_variance);
      void filter(float measured_angle, float measured_rate, float dt);

      float operator ()() const { return x1; }
      float rate() const { return actual_rate; }
    };
    
    Kalman yaw, pitch, roll;
    
    Estimate() { };
    void init();
    void run();
  };
    
  Attitude() { }
  inline void init() { estimate.init(); }
  
  Measure measure;
  Estimate estimate;
};

#endif
