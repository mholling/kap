#include "app.h"
#include "serial.h"
#include <avr/io.h>
#include <stdio.h>

Serial::Serial() {
  UBRR0 = F_CPU / 9600 / 16 - 1;
	UCSR0C = (3 << UCSZ00);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void Serial::send() {
  for (int n = 0; buffer[n] != '\0'; n++) {
    while (!(UCSR0A & (1 << UDRE0))) ;
  	UDR0 = buffer[n];
  }
}

void Serial::debug(const char * const s, char b) {
  sprintf(buffer, "%15s: 0x%02x\r\n", s, b);
  send();
}

void Serial::debug(const char * const s, bool b) {
  sprintf(buffer, "%15s: %s\r\n", s, b ? "true" : "false");
  send();
}

void Serial::debug(const char * const s, int b) {
  sprintf(buffer, "%15s: %i\r\n", s, b);
  send();
}

void Serial::debug_b(const char * const s, char b) {
  char binary[9];
  for (int n = 0; n < 8; n++)
    binary[n] = ((b & _BV(7 - n)) ? '1' : '0');
  binary[8] = '\0';
  sprintf(buffer, "%15s: 0b%s\r\n", s, binary);
  send();
}

void Serial::debug(const char * const s, float f) {
  sprintf(buffer, "%15s: %.3g\r\n", s, (double)f);
  send();
}

void Serial::debug(const char * const s, const Vector v) {
  sprintf(buffer, "%15s: (%.5g, %.5g, %.5g)\r\n", s, (double)v[0], (double)v[1], (double)v[2]);
  send();
}

void Serial::debug(float f) {
  sprintf(buffer, "%+.4e ", (double)f);
  send();
}

void Serial::debug(int i) {
  sprintf(buffer, "%+05i ", i);
  send();
}

void Serial::debug(const Vector v) {
  for (int n = 0; n < 3; n++)
    debug(v[n]);
}

void Serial::line() {
  sprintf(buffer, "\r\n");
  send();
}

