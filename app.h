#ifndef __APP_H_
#define __APP_H_

#include "scheduler.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"
#include "accelerometer.h"
#include "timer.h"
#include "analog.h"
#include "pwm.h"
#include "spi.h"
#include "shift_register.h"
#include "motors.h"
#include "gyros.h"
#include "estimate_attitude.h"
#include "kalman.h"

class App {  
public:
  // Hardware:
  Scheduler scheduler;
  Serial serial;
  I2C i2c;
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  Timer timer;
  // Analog analog;
  // Pwm pwm;
  // Spi spi;
  // ShiftRegister shift_register;
  // Motors motors;
  // Gyros gyros;
  
  // Tasks:
  EstimateAttitude estimate_attitude;
  // Kalman kalman;
  
  void run();
};

// void toggle_led();

extern App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
