#ifndef __ORIENTATION_H_
#define __ORIENTATION_H_

#include "matrix.h"
#include "persistent.h"

class Orientation {
private:
  class Angles {
  public:
    int yaw, pitch, roll;
    void defaults() { yaw = 0; pitch = 0; roll = 3;}
  };
  
  Persistent<Angles> angles;
  
public:
  Orientation() : angles(0xf0) { } // TODO: unified scheme for EEPROM addresses!
  void init();
  
  Vector adjust(const Vector& vector) const;
  Vector variance_from_sd(float sdx, float sdy, float sdz) const;
};

#endif
