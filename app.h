#ifndef __APP_H_
#define __APP_H_

#include "scheduler.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"
#include "timer.h"
#include "analog.h"
#include "pwm.h"
#include "spi.h"
#include "shift_register.h"
#include "motors.h"
#include "gyros.h"

class App {  
public:
  App() { }
  
  Scheduler scheduler;
  Serial serial;
  I2C i2c;
  Magnetometer magnetometer;
  Timer timer;
  Analog analog;
  Pwm pwm;
  Spi spi;
  ShiftRegister shift_register;
  Motors motors;
  Gyros gyros;
  
  void run();
};

extern App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
