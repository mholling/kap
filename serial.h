#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "circular_buffer.h"
#include "safe.h"
#include "vector.h"

class Serial {
protected:
  CircularBuffer<char, 128> buffer;
  
public:
  Serial();
  
  inline void interrupt();
  
  inline void send(const char *data) volatile { Safe<Serial>(this)().send(data); }
  void send(const char *data);
  void debug(const char * const s, char b) volatile;
  void debug(const char * const s, bool b) volatile;
  void debug(const char * const s, int b) volatile;
  void debug_b(const char * const s, char b) volatile;
  void debug(const char * const s, float f) volatile;
  void debug(const char * const s, const Vector v) volatile;
  void debug(float f) volatile;
  void debug(int i) volatile;
  void debug(const Vector v) volatile;
  void line() volatile;
};

#endif
