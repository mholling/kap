#include "motors.h"
#include "pwm.h"
#include "app.h"
#include <avr/io.h>

Motors::Motors() {
  DDRD |= _BV(DDD4) | _BV(DDD5) | _BV(DDD7);
  DDRB |= _BV(DDB0);
}

void Motors::init() {
  enable();
  yaw(-0.5);
  pitch(0.8);
}

void Motors::disable() {
  App::app().shift_register.clear_bit(standby_shift_register_bit);
}

void Motors::enable() {
  App::app().shift_register.set_bit(standby_shift_register_bit);
}

void Motors::yaw(float value) {
  if (yaw_polarity * value > 0) {
    PORTD |= _BV(PIND7);
    PORTB &= ~_BV(PINB0);
  } else {
    PORTD &= ~_BV(PIND7);
    PORTB |= _BV(PINB0);
  }
  App::app().pwm.channel_a.set_duty_cycle(value > 0.0 ? value : -value);
}

void Motors::pitch(float value) {
  if (pitch_polarity * value > 0) {
    PORTD |= _BV(PIND5);
    PORTD &= ~_BV(PINB4);
  } else {
    PORTD &= ~_BV(PIND5);
    PORTD |= _BV(PIND4);
  }
  App::app().pwm.channel_b.set_duty_cycle(value > 0.0 ? value : -value);
}
