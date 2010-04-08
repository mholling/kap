#ifndef __APP_H_
#define __APP_H_

#include "serial.h"
#include "timer.h"
#include "eeprom.h"
#include "orientation.h"
#include "i2c.h"
#include "analog.h"
#include "spi.h"
#include "shift_register.h"
#include "pwm.h"
#include "motors.h"
#include "magnetometer.h"
#include "accelerometer.h"
#include "gyros.h"
#include "trajectory.h"
#include "attitude.h"
#include "pid.h"
#include "diagnostic.h"

class App {  
public:
  App() { }
  
  // Hardware:
  Serial serial;
  Timer timer;
  Eeprom eeprom;
  Orientation orientation;
  I2C i2c;
  Analog analog;
  Spi spi;
  ShiftRegister shift_register;  
  Pwm pwm;
  Motors motors;
  
  // Sensors:
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  Gyros gyros;
  
  // Calculations:
  Trajectory trajectory;
  Attitude attitude;
  Pid pid;
  
  Diagnostic diagnostic;
  
  void run();
};

extern App app;

extern "C" void __cxa_pure_virtual(void);
// extern "C" void atexit(void);

#endif
