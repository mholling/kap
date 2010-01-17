#include "app.h"
#include "scheduler.h"
#include "i2c.h"
#include "magnetometer.h"
#include "timer.h"
#include "analog.h"
#include "pwm.h"
#include "spi.h"
#include "shift_register.h"
#include <avr/io.h>
#include <util/delay.h>


void App::run() {
  scheduler.init();
  i2c.init();
  magnetometer.init();
  timer.init();
  analog.init();
  pwm.init();
  spi.init();
  shift_register.init();
  
  scheduler.run();
}

void toggle_led() {
  DDRB |= _BV(5);
  PORTB ^= _BV(5); 
}

void __cxa_pure_virtual(void) { }
void atexit(void) { }
