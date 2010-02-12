#ifndef __APP_H_
#define __APP_H_

#include "scheduler.h"
#include "serial.h"
#include "i2c.h"
#include "magnetometer.h"
#include "accelerometer.h"
#include "timer.h"
#include "eeprom.h"
#include "analog.h"
#include "pwm.h"
#include "spi.h"
#include "shift_register.h"
#include "motors.h"
#include "gyros.h"
#include "estimate_attitude.h"
#include "kalman_filters.h"
#include "diagnostic_task.h"

class App {  
public:
  // TODO: any better way than using const_cast?
  App() : estimate_attitude(const_cast<Vector&>(accelerometer.measure.vector), const_cast<Vector&>(magnetometer.measure.vector)) { }
  
  // Hardware:
  volatile Scheduler scheduler;
  volatile Serial serial;
  volatile Timer timer;
  volatile Eeprom eeprom;
  volatile I2C i2c;
  volatile Magnetometer magnetometer;
  volatile Accelerometer accelerometer;
  volatile Analog analog;
  volatile Spi spi;
  volatile ShiftRegister shift_register;
  volatile Gyros gyros;
  volatile Pwm pwm;
  volatile Motors motors;
  
  // Tasks:

  volatile EstimateAttitude estimate_attitude;
  volatile KalmanFilters kalman_filters;

  volatile DiagnosticTask diagnostic;
  
  void run();
};

// void toggle_led();

extern App app;
// TODO: maybe just have the entire app be volatile instead of all its members?

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);


  
class EepromTest : public Eeprom::Packet {
public:
  char data[6];
  
public:
  EepromTest(unsigned int address) : Eeprom::Packet(address, data, 6) { }
};



#endif
