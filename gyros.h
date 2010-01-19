#ifndef __GYROS_H_
#define __GYROS_H_

class App;
extern App app;

#include "analog.h"
#include "scheduler.h"

class Gyros {
private:
  enum { power_down_shift_register_bit = 6, self_test_shift_register_pin = 7 };
  class FakeChannel : public Analog::Channel {
  public:
    FakeChannel(unsigned int value) : Analog::Channel(0x0f) { data = value; }
  } fixed;

public:
  Gyros();
  void init();
  
  void disable();
  void enable();
  void normal_mode();
  void test_mode();
  
  class Gyro {
  private:
    Analog::Channel& value;
    Analog::Channel& reference;
    const unsigned int range;
    
  public:
    Gyro(Analog::Channel& value, Analog::Channel& reference, unsigned int range) : value(value), reference(reference), range(range) { }
    float operator ()();
    bool pending() { CriticalSection cs; return value.pending() || reference.pending(); }
  };
  
  Gyro yaw;
  Gyro pitch;
  Gyro roll;
  
  class Task : public Scheduler::Task {
  public:
    Task() : Scheduler::Task(20) { }
    
    void operator ()();
  } task;
};

#endif
