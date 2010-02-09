#ifndef __VECTOR_SENSOR_H_
#define __VECTOR_SENSOR_H_

#include "scheduler.h"
#include "matrix.h"
#include "vector.h"
#include "app.h"

// TODO: don't really need to inherit from this...
class VectorSensor {
protected:
  class Estimate : public Scheduler::Task {
  protected:
    const Vector& m;
    const float lambda;
    float P0[1], P1[2], P2[3], P3[4];
    float w[4];
    Vector previous;
    
  public:
    Estimate(const Vector& m, float variance) : Scheduler::Task(20), m(m), lambda(0.97) {
      P0[0] = P1[1] = P2[2] = P3[3] = 400.0 * variance;
      P1[0] = P2[0] = P2[1] = P3[0] = P3[1] = P3[2] = 0.0;
       w[0] =  w[1] =  w[2] =  w[3] = 0.0;
    }
    Vector vector;
    Vector bias;
    
    void run() {
      if ((previous - m).sqabs() > 350.0 * 350.0) {
        // TODO: use buckets to pick update vectors
        float Px[4];
        
        Px[0] = 2.0 * (P0[0] * m[0] + P1[0] * m[1] + P2[0] * m[2]) + P3[0];
        Px[1] = 2.0 * (P1[0] * m[0] + P1[1] * m[1] + P2[1] * m[2]) + P3[1];
        Px[2] = 2.0 * (P2[0] * m[0] + P2[1] * m[1] + P2[2] * m[2]) + P3[2];
        Px[3] = 2.0 * (P3[0] * m[0] + P3[1] * m[1] + P3[2] * m[2]) + P3[3];
        
        const float d = lambda + 2.0 * (m[0] * Px[0] + m[1] * Px[1] + m[2] * Px[2]) + Px[3];
        float k[4];
        for (int n = 0; n < 4; n++) k[n] = Px[n] / d;
        
        float e = m.sqabs() - 2.0 * (m[0] * w[0] + m[1] * w[1] + m[2] * w[2]) - w[3];
        
        for (int n = 0; n < 4; n++) w[n] += k[n] * e;
        
        for (int n = 0; n < 4; n++) P3[n] = (P3[n] - Px[3] * k[n]) / lambda;
        for (int n = 0; n < 3; n++) P2[n] = (P2[n] - Px[2] * k[n]) / lambda;
        for (int n = 0; n < 2; n++) P1[n] = (P1[n] - Px[1] * k[n]) / lambda;
        for (int n = 0; n < 1; n++) P0[n] = (P0[n] - Px[0] * k[n]) / lambda;
        
        bias = Vector(w[0], w[1], w[2]);
        previous = m;
      }
      vector = m - bias;
    }
  };
    
public:  
  VectorSensor(const Vector& m, float variance) : estimate(m, variance) { }
  
  Estimate estimate;
  inline const Vector& estimated() const { return estimate.vector; }
  inline const Vector& bias() const { return estimate.bias; }
};

#endif





// #ifndef __VECTOR_SENSOR_H_
// #define __VECTOR_SENSOR_H_
// 
// #include "scheduler.h"
// #include "matrix.h"
// #include "vector.h"
// #include "timed_section.h"
// #include "app.h"
// 
// // TODO: don't really need to inherit from this...
// class VectorSensor {
// protected:
//   class Estimate : public Scheduler::Task {
//   protected:
//     const Vector& m;
//     const float lambda;
//     float P0[1], P1[2], P2[3], P3[4];
//     float w[4];
//     Vector previous;
//     
//   public:
//     Estimate(const Vector& m, float variance) : Scheduler::Task(20), m(m), lambda(0.95) {
//       P0[0] = P1[1] = P2[2] = P3[3] = 400.0 * variance;
//       P1[0] = P2[0] = P2[1] = P3[0] = P3[1] = P3[2] = 0.0;
//        w[0] =  w[1] =  w[2] =  w[3] = 0.0;
//     }
//     Vector vector;
//     Vector bias;
//     float field;
//     
//     void run() {
//       if ((previous - m).sqabs() > 350.0*350.0) {
//         // TODO: use buckets to pick update vectors
//         // TimedSection ts("estimate");
//         float Px[4];
//         
//         Px[0] = 2.0 * (P0[0] * m[0] + P1[0] * m[1] + P2[0] * m[2]) + P3[0];
//         Px[1] = 2.0 * (P1[0] * m[0] + P1[1] * m[1] + P2[1] * m[2]) + P3[1];
//         Px[2] = 2.0 * (P2[0] * m[0] + P2[1] * m[1] + P2[2] * m[2]) + P3[2];
//         Px[3] = 2.0 * (P3[0] * m[0] + P3[1] * m[1] + P3[2] * m[2]) + P3[3];
//         
//         float d = lambda + 2.0 * (m[0] * Px[0] + m[1] * Px[1] + m[2] * Px[2]) + Px[3];
//         float e = m.sqabs() - 2.0 * (m[0] * w[0] + m[1] * w[1] + m[2] * w[2]) - w[3];
//         
//         for (int n = 0; n < 4; n++) w[n] += Px[n] * e / d;
//         
//         for (int n = 0; n < 4; n++) P3[n] = (P3[n] - (Px[3] * Px[n]) / d) / lambda;
//         for (int n = 0; n < 3; n++) P2[n] = (P2[n] - (Px[2] * Px[n]) / d) / lambda;
//         for (int n = 0; n < 1; n++) P1[n] = (P1[n] - (Px[1] * Px[n]) / d) / lambda;
//         for (int n = 0; n < 1; n++) P0[n] = (P0[n] - (Px[0] * Px[n]) / d) / lambda;
//         
//         bias = Vector(w[0], w[1], w[2]);
//         field = sqrt(w[3] + bias.sqabs()); // TODO: eventually not needed
//         previous = m;
//       }
//       vector = m - bias;
//     }
//   };
//     
// public:  
//   VectorSensor(const Vector& m, float variance) : estimate(m, variance) { }
//   
//   Estimate estimate;
//   inline const Vector& estimated() const { return estimate.vector; }
//   inline const Vector& bias() const { return estimate.bias; }
//   inline const float field() const { return estimate.field; }
// };
// 
// #endif






// #ifndef __VECTOR_SENSOR_H_
// #define __VECTOR_SENSOR_H_
// 
// #include "scheduler.h"
// #include "matrix.h"
// #include "vector.h"
// #include "timed_section.h"
// 
// class VectorSensor {
// protected:
//   virtual const Vector& measured() const = 0;
//   
//   class Estimate : public Scheduler::Task {
//   protected:
//     const VectorSensor& vector_sensor;
//     const float lambda;
//     Matrix<4, 4> P; // TODO: eventually these matrices will be persisted to EEPROM
//     Matrix<4, 1> w;
//     Vector previous;
//     
//   public:
//     Estimate(const VectorSensor& vector_sensor, float variance) : Scheduler::Task(20), vector_sensor(vector_sensor), lambda(0.95) {
//       P(0,0) = P(1,1) = P(2,2) = P(3,3) = 400.0 * variance;
//     }
//     
//     Vector vector;
//     Vector bias;
//     float field;
//     
//     void run() {
//       if ((previous - vector_sensor.measured()).sqabs() > 350.0*350.0) {
//         // TODO: use buckets to pick update vectors
//         // TODO: use symmetric matrices
//         
//         TimedSection ts("estimation");
// 
//         Matrix<1, 4> xt;
//         xt[0] = 2.0 * vector_sensor.measured()[0];
//         xt[1] = 2.0 * vector_sensor.measured()[1];
//         xt[2] = 2.0 * vector_sensor.measured()[2];
//         xt[3] = 1.0;
//         
//         Matrix<4, 1> Px = P * xt.t();
//         Matrix<4, 1> K = Px / (lambda + (xt * Px)(0,0));
// 
//         w += K * (vector_sensor.measured().sqabs() - (xt * w)(0,0));
//         P -= K * xt * P;
//         P /= lambda;
// 
//         bias = Vector(w[0], w[1], w[2]);
//         field = sqrt(w[3] + bias.sqabs()); // TODO: eventually not needed
//         previous = vector_sensor.measured();
//       }
//       vector = vector_sensor.measured() - bias;
//     }
//   };
//     
// public:  
//   VectorSensor(float variance) : estimate(*this, variance) { }
//   
//   Estimate estimate;
//   inline const Vector& estimated() const { return estimate.vector; }
//   inline const Vector& bias() const { return estimate.bias; }
//   inline const float field() const { return estimate.field; }
// };
// 
// #endif
