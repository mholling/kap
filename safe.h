#ifndef __SAFE_H_
#define __SAFE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

template <typename T>
class Safe {
private:
  Safe(const Safe&);
  Safe& operator =(const Safe&);
  
  T& t;
  const unsigned char sreg;
  
public:
  inline Safe(volatile T& t) : t(const_cast<T&>(t)), sreg(SREG) { cli(); }
  inline Safe(volatile T* t) : t(*const_cast<T*>(t)), sreg(SREG) { cli(); }
  inline ~Safe() { SREG = sreg; }
  
  inline T& operator ()() { return t; }
};

#endif
