#include "kalman_filters.h"
#include "app.h"

KalmanFilters::KalmanFilters() : yaw(app.gyros.yaw), pitch(app.gyros.pitch), roll(app.gyros.roll) { }

KalmanFilters::Task::Task(Gyros::Gyro& gyro) :
   Scheduler::Task(20),
   gyro(gyro),
   x1(0.0), x2(0.0), x3(0.0),
   dt(1.0 / Timer::frequency),
   q1(0.2 * dt), q2(0.2), q3(0.1),
   q11(q1 * q1), q12(q1 * q2), q13(q1 * q3),
   q21(q2 * q1), q22(q2 * q2), q23(q2 * q3),
   q31(q3 * q1), q32(q3 * q2), q33(q3 * q3),
   r11(), r22()
{
     
}  

void KalmanFilters::Task::run() {
  z1 = gyro();
  // z2 = magneto();
  predict();
  update();
}

void KalmanFilters::Task::predict() {
  x2  = z1 - x3; // rate = gyro - bias
  x1 += dt * x2; // angle += dt * rate
  
  float t1 = p11 - dt * p31;
  float t2 = p13 - dt * p33;
  float t3 = p31 - dt * p33;
  float t4 = p33;
  
  p11 = q11 + t1 - dt * t2;
  p12 = q12 - t1;
  p13 = q13 + t2;
  p21 = q21 - t3;
  p22 = q22 + t4;
  p23 = q23 - t4;
  p31 = q31 + t3;
  p32 = q32 - t4;
  p33 = q33 + t4;
}

void KalmanFilters::Task::update() {
  float hphr11 = p22 + p32 + p23 + p33 + r11;
  float hphr12 = p21 + p31;
  float hphr21 = p12 + p13;
  float hphr22 = p11 + r22;
  
  float d = hphr11 * hphr22 - hphr12 * hphr21;
  float t11 =  hphr22 / d;
  float t12 = -hphr12 / d;
  float t21 = -hphr21 / d;
  float t22 =  hphr11 / d;
  
  float k11 = p11 * t21 + (p12 + p13) * t11;
  float k21 = p21 * t21 + (p22 + p23) * t11;
  float k31 = p31 * t21 + (p32 + p33) * t11;
  float k12 = p11 * t22 + (p12 + p13) * t12;
  float k22 = p21 * t22 + (p22 + p23) * t12;
  float k32 = p31 * t22 + (p32 + p33) * t12;
  
  float i1 = z1 - x2 - x3;
  float i2 = x1;
  
  x1 += k11 * i1 + k12 * i2;
  x2 += k21 * i1 + k22 * i2;
  x3 += k31 * i1 + k32 * i2;
  
  float dp11 = k12 * p11 + k11 * (p21 + p31);
  float dp21 = k22 * p11 + k21 * (p21 + p31);
  float dp31 = k32 * p11 + k31 * (p21 + p31);
  float dp12 = k12 * p12 + k11 * (p22 + p32);
  float dp22 = k22 * p12 + k21 * (p22 + p32);
  float dp32 = k32 * p12 + k31 * (p22 + p32);
  float dp13 = k12 * p13 + k11 * (p23 + p33);
  float dp23 = k22 * p13 + k21 * (p23 + p33);
  float dp33 = k32 * p13 + k31 * (p23 + p33);
  
  p11 -= dp11;
  p21 -= dp21;
  p31 -= dp31;
  p12 -= dp12;
  p22 -= dp22;
  p32 -= dp32;
  p13 -= dp13;
  p23 -= dp23;
  p33 -= dp33;
}
