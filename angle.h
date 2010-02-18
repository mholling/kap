#ifndef __ANGLE_H_
#define __ANGLE_H_

#include "quaternion.h"

class Angle {
public:
  typedef float (Quaternion::*method_type)() const;
  
private:
  const Quaternion& quaternion;
  const method_type method;
  
public:
  Angle(const Quaternion& quaternion, method_type method) : quaternion(quaternion), method(method) { }
  float operator ()() const { return (quaternion.*method)(); }
};

#endif
