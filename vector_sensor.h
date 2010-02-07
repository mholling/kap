#ifndef __VECTOR_SENSOR_H_
#define __VECTOR_SENSOR_H_

#include "vector.h"
#include "matrix.h"
#include "scheduler.h"

class VectorSensor {
protected:
  virtual const Vector& measured() const = 0;
  
  class Estimate : public Scheduler::Task {
  protected:
    const VectorSensor& vector_sensor;
    Matrix<4, 4> P;
    Matrix<4, 1> x;
    const float variance;
    
  public:
    Estimate(const VectorSensor& vector_sensor, float variance) : Scheduler::Task(20), vector_sensor(vector_sensor), variance(variance) { }
    Vector vector;    
    void run() {
      Matrix<1, 4> H = vector_sensor.measured().t() * 2.0 << 1.0;
      Matrix<4, 1> K = P * H.t() / (H * P * H.t() + variance);
      Matrix<1, 1> z(vector_sensor.measured().norm_squared());
      x += K * (z - H * x);
      P -= K * H;
      vector = vector_sensor.measured() - Vector(x(0,0), x(1,0), x(2,0));
    }
  };
    
public:  
  VectorSensor(float variance) : estimate(*this, variance) { }
  
  Estimate estimate;
  inline const Vector& estimated() const { return estimate.vector; };
};

#endif
