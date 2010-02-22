#ifndef __ATTITUDE_H_
#define __ATTITUDE_H_

#include "timer.h"
#include "vector_packet.h"
#include "quaternion.h"

class Attitude {
public:
  class Measure : public Timer::Task {
  private:
    const VectorPacketBase& gravity;
    const Vector& magnetism;
    const float a1, a2; // TODO: normalise these weights around a1?
    
    Quaternion quaternion;
    
  public:
    Measure(const VectorPacketBase& gravity, const Vector& magnetism) : gravity(gravity), magnetism(magnetism), a1(1.0), a2(2.0) { }
    void run();
    
    typedef float (Measure::*angle_method_type)() const;    
    float   yaw() const { return quaternion.yaw(); }
    float pitch() const { return quaternion.pitch(); }
    float  roll() const { return quaternion.roll(); }
  };
    
  Attitude();
  void init() { }
  
  Measure measure;
};

#endif
