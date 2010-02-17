#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "circular_buffer.h"
#include "vector.h"

class Serial {
private:
  CircularBuffer<char, 64> buffer;

  void send(const char *data);
    
public:
  Serial();
  
  void init() volatile { }
  
  inline void interrupt();
  
  void send(const char *data) volatile;
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
