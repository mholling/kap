#include "timer.h"
#include <avr/io.h>
#include "app.h"

Timer::Timer() : count(0) {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

unsigned long int Timer::timestamp() {
  return count * OCR2A + TCNT2;
}

void Timer::interrupt(App& app) {
  count++;
  
  if (count % (frequency / 10) == 0) app.diagnostic();

  app.gyros.measure();
  app.accelerometer.measure();
  app.magnetometer.measure();
  app.magnetometer.calibrate();
  app.accelerometer.measure();
  
}

Timer::Diagnostic::Diagnostic(const char *message) : start(app.timer.timestamp()), message(message) { }

Timer::Diagnostic::~Diagnostic() {
  app.serial.debug(message, static_cast<float>(app.timer.timestamp() - start) / (OCR2A * Timer::frequency));
}

ISR(TIMER2_COMPA_vect) {
  App& safe_app = const_cast<App&>(app);
  safe_app.timer.interrupt(safe_app);
}

