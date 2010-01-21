#ifndef __KALMANS_FILTER_H_
#define __KALMANS_FILTER_H_

class App;
extern App app;

#include "scheduler.h"
#include "gyros.h"
#include "timer.h"

class KalmanFilters {
public:
  KalmanFilters();
  void init() { }
  
  class Task : public Scheduler::Task {
  private:
    const Gyros::Gyro& gyro;
    
    float z1, z2;     // measured rate, angle
    float x1, x2, x3; // estimated angle, rate, bias
    const float dt;
    float p11, p12, p13, p21, p22, p23, p31, p32, p33;
    const float q1, q2, q3, q11, q12, q13, q21, q22, q23, q31, q32, q33;
    const float r11, r22;
    
    void predict();
    void update();

  public:
    Task(Gyros::Gyro& gyro);
    void run();
  };
  
  Task yaw;
  // Task pitch;
  // Task roll;
};

#endif
