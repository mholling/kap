#include "app.h"
#include <avr/io.h>
#include <util/delay.h>

void App::run() {
  scheduler.init();
  i2c.init();
  magnetometer.init();
  timer.init();
  
  scheduler.run();
}

// void delay(int t) { for (int n = t; n >= 128; n -= 128) _delay_ms(128); _delay_ms(t % 128); }

void toggle_led() {
  DDRB |= _BV(5);
  PORTB ^= _BV(5); 
}

void __cxa_pure_virtual(void) { }
void atexit(void) { }
