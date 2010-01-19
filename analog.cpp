#include "analog.h"
#include <avr/io.h>
#include "scheduler.h"
#include "app.h"

Analog::Analog() : yaw(0), pitch(1), roll(2), ref(3) {
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  DIDR0 = _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
}

inline const void Analog::interrupt() { // TODO: SPI, I2C etc should be inline here too!
  Channel::head().interrupt();
}

void Analog::start_conversions() {
  yaw.enqueue();
  pitch.enqueue();
  roll.enqueue();
  ref.enqueue();
}

void Analog::Channel::enqueue() {
  CriticalSection cs;
  Queable<Channel>::enqueue();
  if (at_head()) start();
}

void Analog::Channel::dequeue() {
  CriticalSection cs;
  Queable<Channel>::dequeue();
  if (any()) head().start();
}

void Analog::Channel::start() {
  ADMUX = _BV(REFS0) | (channel & 0x0f);
  ADCSRA |= _BV(ADSC);
}

void Analog::Channel::interrupt() {
  data = ADCL + (ADCH << 8);
  dequeue();
}

ISR(ADC_vect) {
  app.analog.interrupt();
}
