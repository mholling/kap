#include "app.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void App::run() {
  sei();
  
  serial.init();
  timer.init(); // TODO: should be inited last to make sure to premature tasks run...
  eeprom.init();
  orientation.init();
  i2c.init();
  analog.init();
  spi.init();
  shift_register.init();
  pwm.init();
  motors.init();

  magnetometer.init();
  accelerometer.init();
  gyros.init();
  
  // trajectory.init();
  attitude.init();
  // pid.init();
  
  while (true) { }
}

App app;

void __cxa_pure_virtual(void) { } // needed for pure virtual methods
// void atexit(void) { }
