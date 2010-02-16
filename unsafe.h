#ifndef __UNSAFE_H_
#define __UNSAFE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

template <typename T>
class Unsafe {
private:
  Unsafe(const Unsafe&);
  Unsafe& operator =(const Unsafe&);
  
  volatile T& t;
  const unsigned char sreg;
  
public:
  inline Unsafe(T& t) : t(const_cast<volatile T&>(t)), sreg(SREG) { sei(); }
  inline Unsafe(T* t) : t(*const_cast<volatile T*>(t)), sreg(SREG) { sei(); }
  inline ~Unsafe() { SREG = sreg; }
  
  inline volatile T& operator ()() { return t; }
};

#endif
