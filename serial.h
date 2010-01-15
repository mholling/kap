#ifndef __SERIAL_H_
#define __SERIAL_H_

class Serial {
protected:
  static char lookup[];
  int blips;
public:
  Serial();
  void character(char c);
  void string(const char * const s);
  void boolean(bool b);
  void binary(char b);
  void hex(char b);
  void integer(int i);
  void newline();
  void debug(const char * const s, char b);
  void debug(const char * const s, bool b);
  void debug(const char * const s, int b);
  void debug_b(const char * const s, char b);
  void blip();
};

#endif