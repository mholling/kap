#include "motors.h"
#include <avr/io.h>
#include "critical_section.h"
#include "pwm.h"

Motors::Motors() :
    disable(standby_shift_register_pin, false),
    enable(standby_shift_register_pin, true),
    yaw(OCR1A, PORTD, PIND7, PORTD, PIND6, false),
    pitch(OCR1B, PORTB, PINB0, PORTD, PIND5, false) {
  DDRD |= _BV(DDD5) | _BV(DDD6) | _BV(DDD7);
  DDRB |= _BV(DDB0);
}

void Motors::Motor::set(float value) {
  CriticalSection cs;
  if (polarity * value > 0.0) {
    in1_reg |= in1_mask;
    in2_reg &= ~in2_mask;    
  } else {
    in2_reg |= in2_mask;
    in1_reg &= ~in1_mask;    
  }
  value = value < 0.0 ? -value : value;
  pwm_reg = (value >= 1.0) ? Pwm::resolution - 1 : static_cast<unsigned int>(value * (Pwm::resolution - 1));
}
