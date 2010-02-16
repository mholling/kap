#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "circular_buffer.h"
#include "vector.h"

class Serial {
protected:
  CircularBuffer<char, 64> buffer;
  
private:
  void send(const char *data);
  
public:
  Serial();
  
  inline void interrupt();
  
  void send(const char *data) volatile { Safe<Serial>(this)().send(data); }
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
