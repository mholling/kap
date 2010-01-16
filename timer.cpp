#include "timer.h"
#include <avr/io.h>
#include "app.h"
#include "analog.h"

Timer::Timer() {
  TCCR2A = _BV(WGM21);
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / tick_frequency;
  TIMSK2 = _BV(OCIE2A); 
}

void Timer::init() {
}

void Timer::interrupt() {
  App& app = App::app();
  app.analog.start_conversions();

  // signal other timer tasks here...
}

ISR(TIMER2_COMPA_vect) {
  App::app().timer.interrupt();
}

