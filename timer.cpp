#include "timer.h"
#include <avr/io.h>
#include "app.h"

Timer::Timer() : count(0) {
  TCCR0B |= _BV(CS02) | _BV(CS00);
  TIMSK0 |= _BV(TOIE0); 
}

void Timer::init() {
  
}

void Timer::interrupt() {
  if (++count > 30) {
    count = 0;
    App::app().scheduler.signal(task);
  }
}

void Timer::Task::operator ()() {
  toggle_led();
  App::app().magnetometer.show_bearing();
}

ISR(TIMER0_OVF_vect) {
  App::app().timer.interrupt();
}
