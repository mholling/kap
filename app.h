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
#include "kalman_filters.h"

class App {  
public:
  // Hardware:
  Scheduler scheduler;
  Serial serial;
  Timer timer;
  I2C i2c;
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  // Analog analog;
  // Spi spi;
  // ShiftRegister shift_register;
  // Gyros gyros;
  // Pwm pwm;
  // Motors motors;
  
  // Tasks:
  EstimateAttitude estimate_attitude;
  // KalmanFilters kalman_filters;
  
  void run();
};

// void toggle_led();

extern App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);

#endif
