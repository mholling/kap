#ifndef __KALMAN_FILTERS_H_
#define __KALMAN_FILTERS_H_

#include "scheduler.h"
#include "gyros.h"

class KalmanFilters {
private:
  class Filter : public Scheduler::Task {
  private:
    const Gyros::Gyro& gyro;
    const volatile float& measured;
  
    float z1, z2;     // measured rate, angle
    float x1, x2, x3; // estimated angle, rate, bias
    const float dt;
    float p11, p12, p13, p21, p22, p23, p31, p32, p33;
    const float q1, q2, q3, q11, q12, q13, q21, q22, q23, q31, q32, q33;
    const float r11, r22;
  
    void predict();
    void update();

  public:
    Filter(const Gyros::Gyro& gyro, const float& measured);
    void run();
    
    float angle() const { return x1; }
    float rate() const { return x2; }
    float bias() const { return x3; }
  };
  
public:
  KalmanFilters();
    
  Filter yaw;
  Filter pitch;
  Filter roll;
};

#endif
