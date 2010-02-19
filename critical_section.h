#ifndef __CRITICAL_SECTION_H_
#define __CRITICAL_SECTION_H_

#include <avr/io.h>
#include <avr/interrupt.h>

class CriticalSection {
private:
  CriticalSection(const CriticalSection&);
  CriticalSection& operator =(const CriticalSection&);
  
  const unsigned char sreg;
  
public:
  inline CriticalSection() : sreg(SREG) { cli(); }
  inline ~CriticalSection() { SREG = sreg; }
};

#endif
