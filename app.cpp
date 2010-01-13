#include "app.h"
#include <avr/io.h>
#include <util/delay.h>

void App::run() {
  serial.string("hello, world!\r\n");
  magnetometer.configure();
  magnetometer.wake();
  // magnetometer.show_diagnostic();
  for (;;) {
    delay(1000);
    magnetometer.show_status();
    magnetometer.show_bearing();
  }
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

extern "C" void atexit(void) { } 
