#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <math.h>

class Vector {
public:
  float x;
  float y;
  float z;
  
  Vector() { }
  Vector(float x, float y, float z) : x(x), y(y), z(z) { }
  Vector(const Vector& p, const Vector& q) { (*this) = p; (*this) *= q; }
  
  Vector& operator *=(const Vector& vector) {
    float _x = y * vector.z - z * vector.y;
    float _y = z * vector.x - x * vector.z;
    float _z = x * vector.y - y * vector.x;
    x = _x;
    y = _y;
    z = _z;
    return *this;
  }
  
  inline Vector& operator +=(const Vector& vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
  }

  inline Vector& operator -=(const Vector& vector) {
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
  }
  
  inline Vector& operator *=(float a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
  }
  
  inline Vector& operator /=(float a) {
    x /= a;
    y /= a;
    z /= a;
    return *this;
  }
  
  inline float operator *(const Vector& vector) const { return x * vector.x + y * vector.y + z * vector.z; }
  inline float norm_squared() const { return (*this) * (*this); }
  inline float norm() const { return sqrt(norm_squared()); }
  
  Vector& normalise() {
    float _norm = norm();
    x /= _norm;
    y /= _norm;
    z /= _norm;
    return *this;
  }
};

#endif
