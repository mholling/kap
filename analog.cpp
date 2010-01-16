#include "analog.h"
#include <avr/io.h>
#include "scheduler.h"
#include "app.h"

Analog::Analog() {
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  DIDR0 = _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
}

void Analog::init() {
}

void Analog::start_conversions() {
  ADMUX = _BV(REFS0);
  ADCSRA |= _BV(ADSC);
}

void Analog::interrupt() {
  unsigned int channel = ADMUX & 0x03;
  data[channel] = ADCL + (ADCH << 8);
  if (channel == 3) {
    // signal conversions are complete...
  } else {
    ADMUX++;
    ADCSRA |= _BV(ADSC);
  }
}

void Analog::ShowResults::operator()() {
  App::app().serial.debug("channel 0", (int)App::app().analog.data[0]);
}

ISR(ADC_vect) {
  App::app().analog.interrupt();
}
