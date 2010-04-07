#include "app.h"
#include <avr/io.h>

void App::run() {
  scheduler.init();
  serial.init();
  timer.init(); // TODO: should be inited last to make sure to premature tasks run...
  eeprom.init();
  orientation.init();
  i2c.init();
  magnetometer.init();
  accelerometer.init();
  analog.init();
  spi.init();
  shift_register.init();
  gyros.init();
  pwm.init();
  motors.init();
  
  // trajectory.init();
  attitude.init();
  // pid.init();
  
  // scheduler.run();
  
  while (true) { }
}

App app;

void __cxa_pure_virtual(void) { } // needed for pure virtual methods
// void atexit(void) { }
