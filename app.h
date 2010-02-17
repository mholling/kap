#ifndef __APP_H_
#define __APP_H_

#include "scheduler.h"
#include "serial.h"
#include "timer.h"
#include "eeprom.h"
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

class App {  
public:
  App() { }
  
  // Hardware:
  Scheduler scheduler;
  volatile Serial serial;
  volatile Timer timer;
  Eeprom eeprom;
  I2C i2c;
  Magnetometer magnetometer;
  Accelerometer accelerometer;
  Analog analog;
  Gyros gyros;
  Spi spi;
  volatile ShiftRegister shift_register;  
  Pwm pwm;
  Motors motors;
  
  // Calculations:
  Attitude attitude;
  
  void run();
};

// void toggle_led();

extern App app;

extern "C" void __cxa_pure_virtual(void);
extern "C" void atexit(void);


// class EepromTest : public Eeprom::Packet {
// public:
//   char data[6];
//   
// public:
//   EepromTest(unsigned int address) : Eeprom::Packet(address, data, 6) { }
// };


#endif
