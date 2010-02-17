#ifndef __ANGLE_H_
#define __ANGLE_H_

#include "quaternion.h"
#include "safe.h"

class Angle {
public:
  typedef float (Quaternion::*method_type)() const;
  
private:
  const volatile Quaternion& quaternion; // TODO: remove volatile?
  const method_type method;
  
public:
  Angle(const volatile Quaternion& quaternion, method_type method) : quaternion(quaternion), method(method) { }
  float operator ()() const volatile { return ((*Safe<const Quaternion>(quaternion)).*method)(); }
};

#endif
