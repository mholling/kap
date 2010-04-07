#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_

#include "timer.h"
#include "vector_packet.h"
#include "quaternion.h"

class Attitude {
public:
  class Measure : public Timer::Task {
  private:
    const float a1, a2; // TODO: normalise these weights around a1?
    
    Quaternion quaternion;
    
  public:
    Measure() : a1(1.0), a2(0.5) { }
    void run();
    
    float   yaw() const { return quaternion.yaw(); }
    float pitch() const { return quaternion.pitch(); }
    float  roll() const { return quaternion.roll(); }
  };
  
  class Estimate : public Timer::Task {
    class Kalman {
    private:
      float x1, x2;
      float P11, P12, P22;
      float Q11, Q12, Q22;
      float R;

      float old_angle;
      int revolutions;

    public:
      // Kalman(float angle_sd, float rate_sd, float bias_sd);
      // Kalman();
      Kalman() : old_angle(0.0), revolutions(0) { }
      
      void set_variances(float angle_variance, float rate_variance, float bias_variance);
      void filter(float measured_angle, float measured_rate);

      float operator ()() const { return x1; }
      float rate() const { return - x2; }
    };
    
  public:
    Kalman yaw, pitch, roll;
    
    Estimate() { };
    // Estimate() : yaw(0.9, 0.012, 0.1), pitch(0.5, 0.008, 0.1), roll(0.5, 0.008, 0.1) { }
    void init();
    void run();
  };
    
  Attitude() { }
  void init();
  
  Measure measure;
  Estimate estimate;
};

#endif
