#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_

#include "scheduler.h"
#include "vector.h"
#include "quaternion.h"

class Attitude {
public:
  class Initiate : public Scheduler::Task {
  public:
    Initiate() : Scheduler::Task(20) { }
    void run();  
  };
  
  class Measure : public Scheduler::Task {
  private:
    const Vector& gravity;
    const Vector& magnetism;
    const float a1, a2; // TODO: normalise these weights around a1?
    
    Quaternion quaternion;
    
  public:
    Measure(const Vector& gravity, const Vector& magnetism) : Scheduler::Task(20), gravity(gravity), magnetism(magnetism), a1(1.0), a2(2.0) { }
    void run();
    
    typedef float (Measure::*angle_method_type)() const;    
    float   yaw() const { return quaternion.yaw(); }
    float pitch() const { return quaternion.pitch(); }
    float  roll() const { return quaternion.roll(); }
  };
  
  class Estimate : public Scheduler::Task {
  public:
    Estimate() : Scheduler::Task(20) { }
    void run();
  };
  
  class Control: public Scheduler::Task {
  public:
    Control() : Scheduler::Task(20) { }
    void run();
  };
  
  Attitude();
  void init() { }
  
  Initiate initiate;
  Measure measure;
  Estimate estimate;
  Control control;
};

#endif
