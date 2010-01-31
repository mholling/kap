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

void Timer::interrupt() {
  static unsigned int count = 0;
  count++;

  // if (!app.gyros.task.pending() && (count % 35 == 0)) {
  //   app.gyros.task();
  // }

  // app.motors.yaw.set((float)(count % 250)/250.0);
  // app.motors.pitch.set((float)(count % 250)/250.0);
}

ISR(TIMER2_COMPA_vect) {
  app.timer.interrupt();
}

