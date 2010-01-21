#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#include "scheduler.h"
#include "gyro.h"
#include "timer.h"

class KalmanFilter {
public:
  KalmanFilter() : yaw(app.gyros.yaw), pitch(app.gyros.pitch), roll(app.gyros.roll) { }
  
  class Task : public SchedulerTask {
  private:
    const Gyros::Gyro& gyro;
    
    const float dt;
    float z1, z2;     // measured rate, angle
    float x1, x2, x3; // estimated angle, rate, bias
    float p11, p21, p32, p12, p22, p32, p31, p32, p33;
    const float q1, q2, q3, q11, q12, q13, q21, q22, q23, q31, q32, q33;
    const float r11, r22;
    
    void predict();
    void update();

  public:
    Task(Gyros::Gyro& gyro);
    void run();
  };
  
  Task yaw;
  Task pitch;
  Task roll;
};

#endif
