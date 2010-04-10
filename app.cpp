#include "app.h"
#include <avr/interrupt.h>

void App::run() {
  sei();
  
  serial.init();
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
  
  trajectory.init();
  attitude.init();
  pid.init();
  
  timer.init(); // init timerlast so timed tasks can't preceed other init() calls
    
  while (true) { }
}

App app;

void __cxa_pure_virtual(void) { } // needed for pure virtual methods
// void atexit(void) { }
