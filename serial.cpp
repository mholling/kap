#include "app.h"
#include "serial.h"
#include <avr/io.h>

char Serial::lookup[] = "0123456789ABCDEF";

Serial::Serial(App* app) : Resource(app), blips(0) {
  unsigned int bittimer = (F_CPU / 9600 / 16 ) - 1;
	UBRR0H = (unsigned char) (bittimer >> 8);
	UBRR0L = (unsigned char) bittimer;
	UCSR0C = (3 << UCSZ00);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void Serial::character(char c) {
  while (!(UCSR0A & (1 << UDRE0))) ;
	UDR0 = c;
}

void Serial::string(const char * const s) {
  for (int n = 0; s[n] != '\0'; n++)
    character(s[n]);
}

void Serial::boolean(bool b) {
  b ? string("true") : string("false");
}

void Serial::binary(char b) {
  for (int n = 8; n > 0; character((b & _BV(--n)) == 0 ? '0' : '1')) ;
}

void Serial::hex(char b) {
  character(lookup[(b>>4)&0x0f]);
  character(lookup[(b>>0)&0x0f]);
}

void Serial::integer(int i) {
  if (i < 0) {
    character('-');
    i = -i;
  }
  char buffer[6];
  int n = 0;
  do {
    buffer[n++] = '0' + i % 10;
    i /= 10;
  } while (i > 0);
  for (; n > 0; character(buffer[--n])) ;
}

void Serial::newline() {
  string("\r\n");
}

void Serial::debug(const char * const s, char b) {
  string(s);
  string(": 0x");
  hex(b);
  newline();
}

void Serial::debug(const char * const s, bool b) {
  string(s);
  string(": ");
  boolean(b);
  newline();
}

void Serial::debug(const char * const s, int b) {
  string(s);
  string(": ");
  integer(b);
  newline();
}

void Serial::debug_b(const char * const s, char b) {
  string(s);
  string(": 0b");
  binary(b);
  newline();
}

void Serial::blip() {
  debug("blip", blips++);
}
