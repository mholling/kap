#include "timer.h"
#include <avr/io.h>
#include "app.h"

Timer::Timer() : count(0) {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

unsigned long int Timer::stamp() {
  return count * OCR2A + TCNT2;
}

Timer::Stamp::Stamp() {
  (*this)();
}

void Timer::Stamp::operator ()() volatile {
  value = app.timer.stamp();
}

long int Timer::Stamp::seconds() const volatile {
  return value / (F_CPU / 1024);
}

long int Timer::Stamp::seconds_ago() const volatile {
  return Stamp().seconds() - seconds();
}

bool Timer::Stamp::since(long int duration_in_seconds) const volatile {
  return seconds_ago() < duration_in_seconds;
}

void Timer::interrupt() {
  count++;
  task();
}

void Timer::Task::run() {
  count++;
  
  app.attitude.initiate();
  app.attitude.measure();
  app.attitude.estimate();
  
  if (count % (Timer::frequency / 10) == 0) diagnostic();
}

Timer::Diagnostic::Diagnostic(const char *message) : start(app.timer.stamp()), message(message) { }

Timer::Diagnostic::~Diagnostic() {
  app.serial.debug(message, static_cast<float>(app.timer.stamp() - start) / (OCR2A * Timer::frequency));
}

ISR(TIMER2_COMPA_vect) {
  Safe<Timer>(app.timer)->interrupt();
}

