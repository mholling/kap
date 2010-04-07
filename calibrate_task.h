#ifndef __CALIBRATE_TASK_H_
#define __CALIBRATE_TASK_H_

#include "timer.h"
#include "vector_packet.h"
#include "vector.h"
#include "persistent.h"

class CalibrateTask : public Timer::Task {
protected:
  enum { autosave_minutes = 5 };
  
  class State {
  public:
    float P0[1], P1[2], P2[3], P3[4];
    float w3;
    float h2, b2;
    Vector bias;
    
    void defaults();
  };
  
  const VectorPacket& measure;
  Vector previous;
  const float lambda;
  
public:
  CalibrateTask(const VectorPacket& measure, unsigned int address, float lambda) : measure(measure), lambda(lambda), state(address) { }
  
  void init() { state.init(); }
  void run();
  
  Persistent<State> state; // TODO: make protected eventually
  inline Vector bias() const { return state.bias; }
};

#endif
