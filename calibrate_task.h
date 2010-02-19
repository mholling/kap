#ifndef __CALIBRATE_TASK_H_
#define __CALIBRATE_TASK_H_

#include "scheduler.h"
#include "vector.h"
#include "persistent.h"

class CalibrateTask : public Scheduler::Task {
protected:
  class State {
  public:
    float P0[1], P1[2], P2[3], P3[4];
    float w3;
    float h2, b2;
    Vector bias;
    
    void defaults();
  };
  
  const Vector& measured;
  Vector previous;
  const float lambda;
  
public:
  CalibrateTask(const Vector& measured, unsigned int address, float lambda) : Scheduler::Task(20), measured(const_cast<Vector&>(measured)), lambda(lambda), state(address) { }
  void init() { state.init(); }
  void run();
  
  Persistent<State> state; // TODO: make protected eventually
  Vector vector;
};

#endif
