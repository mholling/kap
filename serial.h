#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "vector.h"

class Serial {
protected:
  char buffer[40];
  void send();

public:
  Serial();
  void debug(const char * const s, char b);
  void debug(const char * const s, bool b);
  void debug(const char * const s, int b);
  void debug_b(const char * const s, char b);
  void debug(const char * const s, float f);
  void debug(const char * const s, Vector v);
  void debug(float f);
  void debug(int i);
  void line();
};

#endif
