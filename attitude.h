#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_
// 
// class App;
// extern App app;

#include "magnetometer.h"
#include "accelerometer.h"
#include "quaternion.h"

class Attitude {
public:
  Attitude() { }
  void init() { }
    
  class EstimateTask : public Scheduler::Task {
  private:
    const Vector r1, r2, r3;
  public:
    Quaternion result;

    EstimateTask() : Scheduler::Task(20), r1(0.0, 0.0, -1.0), r2(0.0, -1.0, 0.0), r3(r1 * r2) { }
    
    void run();
  } estimate;
  
};

#endif
