#include "motors.h"
#include "pwm.h"
#include "app.h"
#include <avr/io.h>

Motors::Motors(App* app) : Resource(app), yaw(OCR1A, PORTD, PIND7, PORTB, PINB0, false), pitch(OCR1B, PORTD, PIND5, PORTD, PIND4, false) {
  DDRD |= _BV(DDD4) | _BV(DDD5) | _BV(DDD7);
  DDRB |= _BV(DDB0);
}

void Motors::init() {
  disable();
}

void Motors::disable() {
  app().shift_register.clear_bit(standby_shift_register_bit);
}

void Motors::enable() {
  app().shift_register.set_bit(standby_shift_register_bit);
}

void Motors::Motor::set(float value) {
  if (polarity * value > 0.0) {
    in1_reg |= in1_mask;
    in2_reg &= ~in2_mask;    
  } else {
    in2_reg |= in2_mask;
    in1_reg &= ~in1_mask;    
  }
  CriticalSection cs;
  pwm_reg = Pwm::resolution * (value < 0.0 ? -value : value);
  // TODO: clamp value to [-1.0,1.0] ?
}