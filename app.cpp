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
  
  EepromTest w1(0xe0);
  EepromTest w2(0xf0);
  
  w1 << (int)27 << (int)11 << (int)74;
  w2 << (int)23 << (int)03 << (int)76;
  
  w1.write();
  w2.write();  
  w1.wait();
  w2.wait();

  for (volatile int m = 0; m < 100; m++)
    for (volatile int n = 0; n < 32000; n++)
      ; // TODO: EEPROM queing not properly working yet!
  
  EepromTest r1(0xe0);
  EepromTest r2(0xf0);
  
  int x1, x2, x3, y1, y2, y3;
  
  r1.read();
  r2.read();  
  r1.wait();
  r2.wait();
  
  for (volatile int m = 0; m < 100; m++)
    for (volatile int n = 0; n < 32000; n++)
      ; // TODO: EEPROM queing not properly working yet!
  
  r1 >> x1 >> x2 >> x3;
  r2 >> y1 >> y2 >> y3;

  app.serial.debug("x1", x1);
  app.serial.debug("x2", x2);
  app.serial.debug("x3", x3);
  app.serial.debug("y1", y1);
  app.serial.debug("y2", y2);
  app.serial.debug("y3", y3);
  
  while (true) { }
}

// void toggle_led() {
//   DDRB |= _BV(5);
//   PORTB ^= _BV(5); 
// }

void __cxa_pure_virtual(void) { }
void atexit(void) { }
