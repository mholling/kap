#ifndef __ESTIMATE_ATTITUDE_H_
#define __ESTIMATE_ATTITUDE_H_

#include "magnetometer.h"
#include "accelerometer.h"
#include "quaternion.h"

class EstimateAttitude : public Scheduler::Task {
private:
  const float a1, a2; // TODO: normalise these weights around a1?
public:
  EstimateAttitude() : Scheduler::Task(20), a1(1.0), a2(2.0) { }
  void run();
  float yaw, pitch, roll;
};

#endif
