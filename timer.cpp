#include "timer.h"
#include <avr/io.h>
#include "critical_section.h"
#include "app.h"

Timer::Timer() : count(0) {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = ocr2a; // = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

void Timer::interrupt() {
  if (count++ < Timer::frequency) return; // TODO: temporary fix, make permanent
  // count++;
  
  app.magnetometer.measure();
  app.accelerometer.measure();
  app.gyros.channels.z.measure();
  app.gyros.channels.y.measure();
  app.gyros.channels.x.measure();
  app.gyros.channels.ref.measure();

  timed_tasks();
}

void Timer::TimedTasks::run() {
  app.magnetometer.calibrate();
  app.attitude.measure();
  app.attitude.estimate();
  
  // app.trajectory();
  // app.pid.yaw();
  // app.pid.pitch();

  app.diagnostic();
}

unsigned long int Timer::stamp() {
  CriticalSection cs;
  return count * OCR2A + TCNT2;
}

Timer::Stamp::Stamp() {
  (*this)();
}

void Timer::Stamp::operator ()() {
  value = app.timer.stamp();
}

long int Timer::Stamp::seconds() const {
  return value / (F_CPU / 1024);
}

long int Timer::Stamp::seconds_ago() const {
  return Stamp().seconds() - seconds();
}

bool Timer::Stamp::since(long int duration_in_seconds) const {
  return seconds_ago() < duration_in_seconds;
}

Timer::Diagnostic::Diagnostic(const char *message) : start(app.timer.stamp()), message(message) { }

Timer::Diagnostic::~Diagnostic() {
  app.serial.debug(message, static_cast<float>(app.timer.stamp() - start) * Timer::dt() / Timer::ocr2a);
}

ISR(TIMER2_COMPA_vect) {
  app.timer.interrupt();
}

