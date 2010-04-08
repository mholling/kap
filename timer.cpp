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

  if (app.pid.pitch.pending())
    app.serial.send("\r\n!\r\n"); // TODO: just for debugging
  else
    timed_tasks();
}

void Timer::TimedTasks::run() {
  app.magnetometer.calibrate();
  app.attitude.measure();
  app.attitude.estimate();
  app.trajectory.calculate();
  app.pid.yaw();
  app.pid.pitch();

  // app.diagnostic();
}

unsigned long int Timer::stamp() {
  CriticalSection cs;
  return count * ocr2a + TCNT2;
}

long int Timer::Interval::seconds() const {
  return value * 1024 / F_CPU;
}

long int Timer::Interval::microseconds() const {
  return value * 1024 / (F_CPU / 1000000);
}

Timer::Stamp::Stamp() {
  (*this)();
}

void Timer::Stamp::operator ()() {
  value = app.timer.stamp();
}

bool Timer::Stamp::since(long int duration_in_seconds) const {
  return (Stamp() - *this).seconds() < duration_in_seconds;
}

Timer::Diagnostic::Diagnostic(const char *message, int seconds) : message(message), report(seconds < 1 || (app.timer.count % (frequency * seconds) == 0)) { }

Timer::Diagnostic::~Diagnostic() {
  if (report) app.serial.debug(message, static_cast<int>((Stamp() - start).microseconds()));
}

const float Timer::dt = static_cast<float>(ocr2a + 1) * 1024 / F_CPU;


ISR(TIMER2_COMPA_vect) {
  app.timer.interrupt();
}

