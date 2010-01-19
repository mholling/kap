#include "timer.h"
#include <avr/io.h>
#include "app.h"
#include "analog.h"

Timer::Timer(App* app) : Resource(app) {
  TCCR2A = _BV(WGM21); // CTC mode
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = F_CPU / 1024 / frequency - 1;
  TIMSK2 = _BV(OCIE2A); 
}

void Timer::init() {
}

void Timer::interrupt() {
  app().analog.start_conversions();

  // signal other timer tasks here...
  
  static unsigned int count = 0;
  count++;

  if (count % 50 == 0) {
    app().magnetometer.show_bearing();
  }
  
  app().motors.yaw.set((float)(count % 250)/250.0);
  app().motors.pitch.set((float)(count % 250)/250.0);
}

ISR(TIMER2_COMPA_vect) {
  app.timer.interrupt();
}

