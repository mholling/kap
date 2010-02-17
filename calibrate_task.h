#ifndef __CALIBRATE_TASK_H_
#define __CALIBRATE_TASK_H_

#include "scheduler.h"
#include "vector.h"

class CalibrateTask : public Scheduler::Task {
protected:
  const Vector& measured;
  Vector previous;
  const float lambda;
  float P0[1], P1[2], P2[3], P3[4];
  float w3;
  float h2, b2;
  
public:
  CalibrateTask(const volatile Vector& measured, float variance, float lambda);
  void run();
  
  Vector bias;
  Vector vector;
};

#endif
