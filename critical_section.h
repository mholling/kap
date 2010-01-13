#ifndef __CRITICAL_SECTION_H_
#define __CRITICAL_SECTION_H_

#include <avr/io.h>
#include <avr/interrupt.h>

class CriticalSection {
private:
  CriticalSection& operator=(const CriticalSection&);
  bool interrupts;
public:
  CriticalSection() : interrupts(SREG & _BV(SREG_I)) { cli(); }
  ~CriticalSection() { if (interrupts) sei(); };
};

#endif
