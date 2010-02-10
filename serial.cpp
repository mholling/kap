#include "serial.h"
#include <avr/io.h>
#include "critical_section.h"
#include <stdio.h>
#include "app.h"

Serial::Serial() {
  UBRR0 = F_CPU / 9600 / 16 - 1;      // 9600 baud rate // TODO: bump this up once working!
	UCSR0C = (3 << UCSZ00);             // 8-bit characters
	UCSR0B = _BV(UDRIE0) | _BV(TXEN0);  // enable transmitter and data-register-empty interrupts
	
}

void Serial::interrupt() {
  if (buffer.any()) buffer >> reinterpret_cast<volatile char &>(UDR0);
}

void Serial::send(const char *data) {
  CriticalSection cs;
  for (const char *c = data; *c != 0; buffer << *c++) ;
}

void Serial::debug(const char * const s, char b) {
  char temp[64];
  sprintf(temp, "%15s: 0x%02x\r\n", s, b);
  send(temp);
}

void Serial::debug(const char * const s, bool b) {
  char temp[64];
  sprintf(temp, "%15s: %s\r\n", s, b ? "true" : "false");
  send(temp);
}

void Serial::debug(const char * const s, int b) {
  char temp[64];
  sprintf(temp, "%15s: %i\r\n", s, b);
  send(temp);
}

void Serial::debug_b(const char * const s, char b) {
  char temp[64];
  char binary[9];
  for (int n = 0; n < 8; n++)
    binary[n] = ((b & _BV(7 - n)) ? '1' : '0');
  binary[8] = '\0';
  sprintf(temp, "%15s: 0b%s\r\n", s, binary);
  send(temp);
}

void Serial::debug(const char * const s, float f) {
  char temp[64];
  sprintf(temp, "%15s: %.3g\r\n", s, (double)f);
  send(temp);
}

void Serial::debug(const char * const s, const Vector v) {
  char temp[64];
  sprintf(temp, "%15s: (%.5g, %.5g, %.5g)\r\n", s, (double)v[0], (double)v[1], (double)v[2]);
  send(temp);
}

void Serial::debug(float f) {
  char temp[64];
  sprintf(temp, "%+.4e ", (double)f);
  send(temp);
}

void Serial::debug(int i) {
  char temp[64];
  sprintf(temp, "%+05i ", i);
  send(temp);
}

void Serial::debug(const Vector v) {
  for (int n = 0; n < 3; n++)
    debug(v[n]);
}

void Serial::line() {
  send("\r\n");
}


ISR(USART_UDRE_vect) {
  app.serial.interrupt();
}
