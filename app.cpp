#include "app.h"
#include <avr/io.h>
#include <util/delay.h>

void App::run() {
  scheduler.init();
  i2c.init();
  magnetometer.init();
  
  scheduler.run();
}

void delay(int t) { for (int n = t; n >= 128; n -= 128) _delay_ms(128); _delay_ms(t % 128); }

void flash() {
  DDRB |= _BV(5);
  PORTB |= 1<<5;
  delay(250);
  PORTB &= ~(1<<5);
  delay(250);
}

void long_flash() {
  DDRB |= _BV(5);
  PORTB |= 1<<5;
  delay(500);
  PORTB &= ~(1<<5);
  delay(500);
}

void __cxa_pure_virtual(void) { }
void atexit(void) { }
