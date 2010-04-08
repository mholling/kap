#ifndef __APP_H_
#define __APP_H_

#include "task.h"
#include "serial.h"
#include "timer.h"
#include "eeprom.h"
#include "orientation.h"
#include "i2c.h"
#include "magnetometer.h"
#include "accelerometer.h"
#include "analog.h"
#include "gyros.h"
#include "spi.h"
#include "shift_register.h"
#include "pwm.h"
#include "motors.h"
#include "attitude.h"
#include "trajectory.h"
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
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  Analog analog;
  Spi spi;
  ShiftRegister shift_register;  
  Gyros gyros;
  Pwm pwm;
  Motors motors;
  
  // Calculations:
  // Trajectory trajectory;
  Attitude attitude;
  // Pid pid;
  Diagnostic diagnostic;
  
  void run();
};

extern App app;

extern "C" void __cxa_pure_virtual(void);
// extern "C" void atexit(void);


// class EepromTest : public Eeprom::Packet {
// public:
//   char data[6];
//   
// public:
//   EepromTest(unsigned int address) : Eeprom::Packet(address, data, 6) { }
// };


#endif
