#ifndef __VECTOR_SENSOR_H_
#define __VECTOR_SENSOR_H_

#include "vector.h"
#include "matrix.h"
#include "scheduler.h"
#include "timed_section.h"

class VectorSensor {
protected:
  virtual const Vector& measured() const = 0;
  
  class Estimate : public Scheduler::Task {
  protected:
    const VectorSensor& vector_sensor;
    const float lambda;
    Matrix<4, 4> P;
    Matrix<4, 1> w;
    Vector previous;
    
  public:
    Estimate(const VectorSensor& vector_sensor, float variance) : Scheduler::Task(20), vector_sensor(vector_sensor), lambda(0.95) {
      P(0,0) = P(1,1) = P(2,2) = P(3,3) = 400.0 * variance;
    }
    
    Vector vector;
    Vector bias;
    float field;
    
    void run() {
      if (((Vector)(previous - vector_sensor.measured())).norm_squared() > 350.0*350.0) {
        // TimedSection ts("rls update");
        // TODO: use buckets to pick update vectors
        // TODO: use symmetric matrices

        Matrix<1, 4> xt = vector_sensor.measured().t() * 2.0 << 1.0;
        Matrix<4, 1> Px = P * xt.t();
        Matrix<4, 1> K = Px / (lambda + (xt * Px)(0,0));
        w += K * (vector_sensor.measured().norm_squared() - (xt * w)(0,0));
        P -= K * xt * P;
        P /= lambda;

        bias = Vector(w(0,0), w(1,0), w(2,0));
        field = sqrt(w(3,0) + bias.norm_squared());
        previous = vector_sensor.measured();
      }
      vector = vector_sensor.measured() - bias;
    }
  };
    
public:  
  VectorSensor(float variance) : estimate(*this, variance) { }
  
  Estimate estimate;
  inline const Vector& estimated() const { return estimate.vector; }
  inline const Vector& bias() const { return estimate.bias; }
  inline const float field() const { return estimate.field; }
};

#endif
