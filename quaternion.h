#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#include <math.h>

class Quaternion {
public:
  Vector v;
  float w;
  
  Quaternion() { }
  Quaternion(const Vector&v, float w) : v(v), w(w) { }
  Quaternion(float x, float y, float z, float w) : v(Vector(x,y,z)), w(w) { }
  
  Quaternion& operator *=(float rhs) {
    v *= rhs;
    w *= rhs;
    return *this;
  }
  
  const Quaternion operator *(float rhs) {
    return Quaternion(*this) *= rhs;
  }
  
  Quaternion& operator *=(const Quaternion& rhs) {
    Vector cross_product = v * rhs.v;
    Vector vector = rhs.v;
    vector *= w;
    w *= rhs.w;
    w -= v % rhs.v;
    v *= rhs.w;
    v += vector;
    v -= cross_product;
    return *this;
  }
  
  const Quaternion operator *(const Quaternion& rhs) {
    return Quaternion(*this) *= rhs;
  }
    
  Quaternion& operator /=(float rhs) {
    v /= rhs;
    w /= rhs;
    return *this;
  }
  
  const Quaternion operator /(const float rhs) {
    return Quaternion(*this) /= rhs;
  }

  inline float norm_squared() const { return v.norm_squared() + w * w; }
  inline float norm() const { return sqrt(norm_squared()); }
  
  Quaternion& normalise() {
    return *this /= norm();
  }
  
  const Quaternion normalised() const {
    return Quaternion(*this).normalise();
  }
  
  const float yaw() {
    return atan2(2 * (w * v.x + v.y * v.z), 1 - 2 * (v.x * v.x + v.y * v.y)) * 180.0 / M_PI;
  }
  
  const float pitch() {
    return asin(2 * (w * v.y - v.z * v.x)) * 180.0 / M_PI;
  }
  
  const float roll() {
    return atan2(2 * (w * v.x + v.y * v.z), 1 - 2 * (v.x * v.x + v.y * v.y)) * 180.0 / M_PI;
  }
};

#endif
