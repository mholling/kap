#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "circular_buffer.h"
#include "vector.h"

class Serial {
private:
  CircularBuffer<char, 64> buffer;
    
public:
  Serial();
  
  void init() { }
  
  inline void interrupt();
  
  void send(const char *data);
  void debug(const char * const s, char b);
  void debug(const char * const s, bool b);
  void debug(const char * const s, int b);
  void debug_b(const char * const s, char b);
  void debug(const char * const s, float f);
  void debug(float f);
  void debug(int i);
  void debug(const Vector v);
  void line();
};

#endif
