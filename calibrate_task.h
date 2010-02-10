#ifndef __CALIBRATE_TASK_H_
#define __CALIBRATE_TASK_H_

#include "scheduler.h"
#include "vector.h"

class CalibrateTask : public Scheduler::Task {
protected:
  Vector& vector;
  Vector previous;
  const float lambda;
  float P0[1], P1[2], P2[3], P3[4];
  float w[4];
  
public:
  CalibrateTask(Vector& vector, float variance, float lambda);
  void run();
  
  Vector bias;
};

#endif
