#ifndef __CALIBRATE_TASK_H_
#define __CALIBRATE_TASK_H_

#include "timer.h"
#include "vector_packet.h"
#include "vector.h"
#include "persistent.h"

class CalibrateTask : public Timer::Task {
protected:
  class State {
  public:
    float P0[1], P1[2], P2[3], P3[4];
    float w3;
    float h2, b2;
    Vector bias;
    
    void defaults();
  };
  
  const VectorPacketBase& measure;
  Vector previous;
  const float lambda;
  
public:
  CalibrateTask(const VectorPacketBase& measure, unsigned int address, float lambda) : measure(measure), lambda(lambda), state(address) { }
  void init() { state.init(); }
  void run();
  
  Persistent<State> state; // TODO: make protected eventually
  Vector vector;
};

#endif
