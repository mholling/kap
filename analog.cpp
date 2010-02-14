#include "analog.h"
#include <avr/io.h>
#include "app.h"

Analog::Analog() : yaw(0), pitch(1), roll(2), ref(3) {
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  DIDR0 = _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
}

inline void Analog::Channel::initiate() {
  ADMUX = _BV(REFS0) | (number & 0x0f);
  ADCSRA |= _BV(ADSC);
}

inline bool Analog::Channel::process() {
  return true;
}

inline void Analog::Channel::terminate() {
  data = ADCL + (ADCH << 8);
}

ISR(ADC_vect) {
  Analog::Channel::head().interrupt();
}
