#ifndef __CRITICAL_SECTION_H_
#define __CRITICAL_SECTION_H_

#include <avr/io.h>
#include <avr/interrupt.h>

class CriticalSection {
private:
  CriticalSection& operator=(const CriticalSection&);
  unsigned char sreg;
public:
  // TODO: check these are working by using serial comms
  CriticalSection() : sreg(SREG) { cli(); }
  ~CriticalSection() { SREG = sreg; };
};

#endif
