#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_

#include "scheduler.h"
#include "vector.h"
#include "quaternion.h"
#include "angle.h"

class Attitude {
private:
  class Initiate : public Scheduler::Task {
  public:
    Initiate() : Scheduler::Task(20) { }
    void run() volatile;  
  };
  
  class Measure : public Scheduler::Task {
  private:
    const volatile Vector& gravity;
    const volatile Vector& magnetism;
    const float a1, a2; // TODO: normalise these weights around a1?
    
    Quaternion quaternion;
    
  public:
    Measure(const volatile Vector& gravity, const volatile Vector& magnetism) : Scheduler::Task(20), gravity(gravity), magnetism(magnetism), a1(1.0), a2(2.0), yaw(quaternion, &Quaternion::yaw), pitch(quaternion, &Quaternion::pitch), roll(quaternion, &Quaternion::roll) { }
    void run() volatile;
  
    Angle yaw, pitch, roll;
  };
  
  class Estimate : public Scheduler::Task {
  private:
    
  public:
    Estimate() : Scheduler::Task(20) { }
    void run() volatile;
  };
  
public:
  Attitude();
  void init() volatile { }
  
  Initiate initiate;
  Measure measure;
  Estimate estimate;
};

#endif
