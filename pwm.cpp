#include "pwm.h"
#include <avr/io.h>
#include "critical_section.h"

Pwm::Pwm(App* app) : Resource(app) {
  // set phase-correct PWM using TOP = ICR1, no pre-scaling, non-inverting outputs:
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(CS10);
  ICR1 = resolution - 1; // set TOP counter value to the resolution, determine by PWM frequency
  DDRB |= _BV(DDB1) | _BV(DDB2); // set PB1 and PB2 as outputs
}

