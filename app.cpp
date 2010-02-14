#include "app.h"
#include <avr/io.h>

void App::run() {
  scheduler.init();
  timer.init();
  eeprom.init();
  i2c.init();
  magnetometer.init();
  accelerometer.init();
  analog.init();
  gyros.init();
  
  // spi.init();
  // shift_register.init();
  // pwm.init();
  // motors.init();
  
  // scheduler.run();
      
  while (true) { }
}

// void toggle_led() {
//   DDRB |= _BV(5);
//   PORTB ^= _BV(5); 
// }

void __cxa_pure_virtual(void) { }
void atexit(void) { }
