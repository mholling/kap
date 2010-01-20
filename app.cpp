#include "app.h"
#include <avr/io.h>

void App::run() {
  scheduler.init();
  i2c.init();
  magnetometer.init();
  timer.init();
  analog.init();
  pwm.init();
  spi.init();
  shift_register.init();
  motors.init();
  gyros.init();
  
  scheduler.run();
}

// void toggle_led() {
//   DDRB |= _BV(5);
//   PORTB ^= _BV(5); 
// }

void __cxa_pure_virtual(void) { }
void atexit(void) { }
