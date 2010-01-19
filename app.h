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

class App {  
public:
  App() : scheduler(this), serial(this), i2c(this), magnetometer(this), timer(this), analog(this), pwm(this), spi(this), shift_register(this), motors(this) { }
  
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
  
  void run();
};

extern App app;

void toggle_led();

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
