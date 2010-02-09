#include "timer.h"
#include <avr/io.h>
#include "app.h"

Timer::Timer() {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

void Timer::interrupt() {
  app.accelerometer.measure();
  app.magnetometer.measure();
  app.estimate_attitude();
  app.magnetometer.estimate();

  static unsigned int count = 0;
  count++;  
  if (count % (frequency / 10) == 0) {
    // app.serial.debug(app.accelerometer.measure.vector);
    // app.serial.debug(Vector(0.0, 0.0, 0.0));
    // app.serial.debug(app.accelerometer.measure.vector.abs());
    // app.serial.debug(app.magnetometer.measured());
    // app.serial.debug(app.magnetometer.bias());
    // app.serial.debug(app.magnetometer.field());
    app.serial.line();
  }
}

ISR(TIMER2_COMPA_vect) {
  app.timer.interrupt();
}

