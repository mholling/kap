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
  // Hardware:
  Scheduler scheduler;
  Serial serial;
  Timer timer;
  Eeprom eeprom;
  I2C i2c;
  Magnetometer magnetometer;
  // Accelerometer accelerometer;
  // Analog analog;
  // Spi spi;
  // ShiftRegister shift_register;
  // Gyros gyros;
  // Pwm pwm;
  // Motors motors;
  
  // Tasks:
  // EstimateAttitude estimate_attitude;
  // KalmanFilters kalman_filters;
  DiagnosticTask diagnostic;
  
  void run();
};

// void toggle_led();

extern App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);


  
class EepromTest : public Eeprom::Packet {
public:
  char data[6];
  
public:
  EepromTest(unsigned int address) : Eeprom::Packet(address, data, 6) { }
};



#endif
