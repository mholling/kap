#ifndef __PID_H_
#define __PID_H_

#include "scheduler.h"
#include "gyros.h"
#include "motors.h"
#include "trajectory.h"

class Pid {
public:
  Pid();
  void init() { yaw.init(); pitch.init(); }
  
  class Controller : public Timer::Task {
  private:
    const Gyros::Gyro& gyro;
    const Trajectory::Angle target;
    Motors::Motor& motor;

    float previous;
    float integral;
      
    const float kp;
    const float ti;
    const float td;
    const float clamp;
    
  public:
    Controller(const Gyros::Gyro& gyro, const Trajectory::Angle target, Motors::Motor& motor) :
      gyro(gyro),
      target(target),
      motor(motor),
      kp(1.0), ti(0.2), td(0.0), clamp(ti / kp) { reset(); }
    void init() { }
    
    void run();

    void reset();
    void update();
  };
  
  Controller yaw;
  Controller pitch;
};

#endif






// #ifndef __PID_H_
// #define __PID_H_
// 
// #include "scheduler.h"
// #include "gyros.h"
// #include "motors.h"
// #include "trajectory.h"
// 
// class Pid {
// public:
//   Pid();
//   void init() { yaw.init(); pitch.init(); }
//   
//   class Controller : public Timer::Task {
//   private:
//     class State {
//     private:
//       const Gyros::Gyro& gyro;
//       const Trajectory::Angle trajectory_angle;
//       float previous;
//       
//     public:
//       State(const Gyros::Gyro& gyro, const Trajectory::Angle trajectory_angle) : gyro(gyro), trajectory_angle(trajectory_angle) { reset(); }
//       
//       void reset();
//       void update();
//       
//       float error;
//       float derivative;
//       float integral;
//     } state;
//     
//     class Gains {
//     private:
//       const State& state;
//       float kp;
//       float ti;
//       float td;
//       
//     public:
//       Gains(const State& state) : state(state) { }
//       
//     } gains;
//     
//     Motors::Motor& motor;
//     const float kp, ki, kd;
//   
//   public:
//     Controller(const Gyros::Gyro& gyro, const Trajectory::Angle trajectory_angle, Motors::Motor& motor) : state(gyro, trajectory_angle), gains(state), motor(motor), kp(1.0), ki(0.2), kd(0.0) { }
//     void init() { }
//     
//     void run();
//   };
//   
//   Controller yaw;
//   Controller pitch;
// };
// 
// #endif
