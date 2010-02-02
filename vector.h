#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <math.h>

// TODO: inline any of these functions?
class Vector {
public:
  float x;
  float y;
  float z;
  
  Vector() { }
  Vector(float x, float y, float z) : x(x), y(y), z(z) { }
  
  Vector& operator *=(const Vector& rhs) {
    float _x = y * rhs.z - z * rhs.y;
    float _y = z * rhs.x - x * rhs.z;
    float _z = x * rhs.y - y * rhs.x;
    x = _x;
    y = _y;
    z = _z;
    return *this;
  }
  
  const Vector operator *(const Vector& rhs) const {
    return Vector(*this) *= rhs;
  }
  
  Vector& operator +=(const Vector& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  
  const Vector operator +(const Vector& rhs) const {
    return Vector(*this) += rhs;
  }

  Vector& operator -=(const Vector& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  
  const Vector operator -(const Vector& rhs) const {
    return Vector(*this) -= rhs;
  }
  
  Vector& operator *=(float a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
  }
  
  const Vector operator *(float rhs) const {
    return Vector(*this) *= rhs;
  }
  
  Vector& operator /=(float rhs) {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }
  
  const Vector operator /(float rhs) const {
    return Vector(*this) /= rhs;
  }
  
  float operator %(const Vector& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
  float norm_squared() const { return (*this) % (*this); }
  float norm() const { return sqrt(norm_squared()); }
  
  Vector& normalise() {
    return *this /= norm();
  }
  
  const Vector normalised() const {
    return Vector(*this).normalise();
  }
};

#endif
