#include "gyros.h"
#include "app.h"

Gyros::Gyros() :
  disable(power_down_shift_register_pin, true),
  enable(power_down_shift_register_pin, false),
  test_mode(self_test_shift_register_pin, true),
  normal_mode(self_test_shift_register_pin, false),
  yaw(channels.yaw, channels.fixed, 300.0, &Attitude::Measure::yaw),
  pitch(channels.pitch, channels.ref, 400.0, &Attitude::Measure::pitch),
  roll(channels.roll, channels.ref, 400.0, &Attitude::Measure::roll) { }
  
float Gyros::Gyro::rate() const {
  reference.wait();
  value.wait();
  return (value() / reference() - 1.0) * range;
}

Gyros::Gyro::Estimate::Estimate(const Gyro& gyro, Attitude::Measure::angle_method_type measured_angle) :
   gyro(gyro),
   measured_angle(measured_angle),
   x1(0.0), x2(0.0), x3(0.0),
   q1(0.2 / Timer::frequency), q2(0.2), q3(0.1), // TODO: values??
   q11(q1 * q1), q12(q1 * q2), q13(q1 * q3),
   q21(q2 * q1), q22(q2 * q2), q23(q2 * q3),
   q31(q3 * q1), q32(q3 * q2), q33(q3 * q3),
   r11(), r22() // TODO: values???
{
}

void Gyros::Gyro::Estimate::run() {
  z1 = gyro.rate();
  z2 = (app.attitude.measure.*measured_angle)();
  predict();
  correct();
}

void Gyros::Gyro::Estimate::predict() {
  // compute  x = A x + B u
  x2  = z1 - x3; // rate = gyro - bias
  x1 += x2 / Timer::frequency; // angle += dt * rate
  
  // compute intermediate values for  A P A'
  float t1 = p11 - p31 / Timer::frequency;
  float t2 = p13 - p33 / Timer::frequency;
  float t3 = p31 - p33 / Timer::frequency;
  float t4 = p33;
  
  // compute  P = A P A' + Q
  p11 = q11 + t1 - t2 / Timer::frequency;
  p12 = q12 - t2;
  p13 = q13 + t2;
  p21 = q21 - t3;
  p22 = q22 + t4;
  p23 = q23 - t4;
  p31 = q31 + t3;
  p32 = q32 - t4;
  p33 = q33 + t4;
}

void Gyros::Gyro::Estimate::correct() {
  // compute  H P H' + R
  float hphr11 = p22 + p32 + p23 + p33 + r11;
  float hphr12 = p21 + p31;
  float hphr21 = p12 + p13;
  float hphr22 = p11 + r22;
  
  // compute  I / (H P H' + R)
  float d = hphr11 * hphr22 - hphr12 * hphr21;
  float t11 =  hphr22 / d;
  float t12 = -hphr12 / d;
  float t21 = -hphr21 / d;
  float t22 =  hphr11 / d;
  
  // compute Kalman gain  K = P H' / (H P H' + R)
  float k11 = p11 * t21 + (p12 + p13) * t11;
  float k21 = p21 * t21 + (p22 + p23) * t11;
  float k31 = p31 * t21 + (p32 + p33) * t11;
  float k12 = p11 * t22 + (p12 + p13) * t12;
  float k22 = p21 * t22 + (p22 + p23) * t12;
  float k32 = p31 * t22 + (p32 + p33) * t12;
  
  // compute  z - H x
  float i1 = z1 - x2 - x3;
  float i2 = z2 - x1;
  
  // compute  x += K (z - H x)
  x1 += k11 * i1 + k12 * i2;
  x2 += k21 * i1 + k22 * i2;
  x3 += k31 * i1 + k32 * i2;
  
  // compute  K H P
  float dp11 = k12 * p11 + k11 * (p21 + p31);
  float dp21 = k22 * p11 + k21 * (p21 + p31);
  float dp31 = k32 * p11 + k31 * (p21 + p31);
  float dp12 = k12 * p12 + k11 * (p22 + p32);
  float dp22 = k22 * p12 + k21 * (p22 + p32);
  float dp32 = k32 * p12 + k31 * (p22 + p32);
  float dp13 = k12 * p13 + k11 * (p23 + p33);
  float dp23 = k22 * p13 + k21 * (p23 + p33);
  float dp33 = k32 * p13 + k31 * (p23 + p33);
  
  // compute  P -= K H P
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
