#include "app.h"
#include <avr/io.h>

void App::run() {
  scheduler.init();
  timer.init();
  eeprom.init();
  i2c.init();
  magnetometer.init();
  // accelerometer.init();
  // analog.init();
  // spi.init();
  // shift_register.init();
  // gyros.init();
  // pwm.init();
  // motors.init();
  
  // scheduler.run();
  
  EepromTest r(0xf8);
  
  int x1 = 0,  x2 = 0,  x3 = 0;
  r.read();

  for (volatile int m = 0; m < 100; m++)
    for (volatile int n = 0; n < 32000; n++)
      ;
  r >> x1 >> x2 >> x3;
  
  app.serial.debug("1", (char)x1);
  app.serial.debug("2", (char)x2);
  app.serial.debug("3", (char)x3);
  
  while (true) { }
}

// void toggle_led() {
//   DDRB |= _BV(5);
//   PORTB ^= _BV(5); 
// }

void __cxa_pure_virtual(void) { }
void atexit(void) { }
