#ifndef __ORIENTATION_H_
#define __ORIENTATION_H_

#include "matrix.h"
#include "persistent.h"

class Orientation {
private:
  Matrix<3, 3> rotation;
  
  class Angles {
  public:
    int yaw, pitch, roll;
    void defaults() { yaw = pitch = roll = 0; roll = 0; } // -90;}
  };
  
  Persistent<Angles> angles;
  
public:
  Orientation() : angles(0xf0) { } // TODO: unified scheme for EEPROM addresses!
  void init();
  
  const Vector adjust_vector(const Vector& vector) const;
  const Vector adjust_variance(float vx, float vy, float vz) const;
};

#endif
