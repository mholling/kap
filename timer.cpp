#include "timer.h"
#include <avr/io.h>
#include "app.h"
#include "analog.h"

Timer::Timer() {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

void Timer::init() {
}

void Timer::interrupt() {
  App& app = App::app();
  app.analog.start_conversions();

  // signal other timer tasks here...

  // static int count = 0;
  // if (++count > 50) {
  //   count = 0;
  //   app.magnetometer.show_bearing();
  // }
}

ISR(TIMER2_COMPA_vect) {
  App::app().timer.interrupt();
}

