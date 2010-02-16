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
  // TODO: any better way than using const_cast? (i.e. review use of at this level)
  
  App() : estimate_attitude(const_cast<Vector&>(accelerometer.measure.vector), const_cast<Vector&>(magnetometer.measure.vector)) { }
  
  // Hardware:
  Scheduler scheduler;
  Serial serial;
  Timer timer;
  Eeprom eeprom;
  I2C i2c;
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  Analog analog;
  Gyros gyros;
  Spi spi;
  ShiftRegister shift_register;  
  Pwm pwm;
  Motors motors;
  
  // Tasks:

  EstimateAttitude estimate_attitude;
  KalmanFilters kalman_filters;
  DiagnosticTask diagnostic;
  
  void run() volatile;
};

// void toggle_led();

extern volatile App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);


class EepromTest : public Eeprom::Packet {
public:
  char data[6];
  
public:
  EepromTest(unsigned int address) : Eeprom::Packet(address, data, 6) { }
};


#endif
