#include "analog.h"
#include <avr/io.h>
#include "app.h"

Analog::Analog() : yaw(0), pitch(1), roll(2), ref(3) {
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  DIDR0 = _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
}

void Analog::Channel::convert() {
  if (enqueue())
    if (at_head()) start();
}

void Analog::Channel::dequeue() {
  Queable<Channel>::dequeue();
  if (any()) const_cast<Channel&>(head()).start();
}

void Analog::Channel::start() {
  ADMUX = _BV(REFS0) | (number & 0x0f);
  ADCSRA |= _BV(ADSC);
}

void Analog::Channel::interrupt() {
  data = ADCL + (ADCH << 8);
  dequeue();
}

ISR(ADC_vect) {
  Analog::Channel::head().interrupt();
}
