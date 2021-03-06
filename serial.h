#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "circular_buffer.h"
#include "vector.h"

class Serial {
private:
  CircularBuffer<char, 128> buffer;
    
public:
  Serial();
  
  inline void init() { }
  
  void interrupt();
  
  void send(const char *data);
  void debug(const char * const s, char b);
  void debug(const char * const s, bool b);
  void debug(const char * const s, int b);
  void debug_b(const char * const s, char b);
  void debug(const char * const s, float f, int dp = 4);
  void debug(float f, int dp = 4);
  void debug(int i);
  void debug(const Vector v);
  void line();
};

#endif
