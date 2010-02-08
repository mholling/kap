#ifndef __QUATERNION_H_
#define __QUATERNION_H_
 
#include "vector.h"
#include <math.h>
 
// TODO: convert to inherit from Matrix<4, 1>?
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
    Vector cross_product = v.cross(rhs.v);
    Vector vector = rhs.v * w;
    w *= rhs.w;
    w -= v.dot(rhs.v);
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
 
  inline float sqabs() const { return v.sqabs() + w * w; }
  inline float abs() const { return sqrt(sqabs()); }
  
  Quaternion& normalise() {
    return *this /= abs();
  }
  
  const Quaternion normalised() const {
    return Quaternion(*this).normalise();
  }
  
  void get_euler_angles(float& yaw, float& pitch, float& roll) const {
    yaw   = atan2(2 * (w * v(2) + v(1) * v(2)), 1 - 2 * (v(1) * v(1) + v(2) * v(2)));
    pitch =  asin(2 * (w * v(1) - v(2) * v(0)));
    roll  = atan2(2 * (w * v(0) + v(1) * v(2)), 1 - 2 * (v(0) * v(0) + v(1) * v(1)));
  }
};
 
#endif
 
 
 
 
// 
// #ifndef __QUATERNION_H_
// #define __QUATERNION_H_
// 
// #include <math.h>
// 
// class Quaternion {
// public:
//   Vector v;
//   float w;
//   
//   Quaternion() { }
//   Quaternion(const Vector&v, float w) : v(v), w(w) { }
//   Quaternion(float x, float y, float z, float w) : v(Vector(x,y,z)), w(w) { }
//   
//   Quaternion& operator *=(float rhs) {
//     v *= rhs;
//     w *= rhs;
//     return *this;
//   }
//   
//   const Quaternion operator *(float rhs) {
//     return Quaternion(*this) *= rhs;
//   }
//   
//   Quaternion& operator *=(const Quaternion& rhs) {
//     Vector cross_product = v * rhs.v;
//     Vector vector = rhs.v;
//     vector *= w;
//     w *= rhs.w;
//     w -= v % rhs.v;
//     v *= rhs.w;
//     v += vector;
//     v -= cross_product;
//     return *this;
//   }
//   
//   const Quaternion operator *(const Quaternion& rhs) {
//     return Quaternion(*this) *= rhs;
//   }
//     
//   Quaternion& operator /=(float rhs) {
//     v /= rhs;
//     w /= rhs;
//     return *this;
//   }
//   
//   const Quaternion operator /(const float rhs) {
//     return Quaternion(*this) /= rhs;
//   }
// 
//   inline float sqabs() const { return v.sqabs() + w * w; }
//   inline float abs() const { return sqrt(sqabs()); }
//   
//   Quaternion& normalise() {
//     return *this /= abs();
//   }
//   
//   const Quaternion normalised() const {
//     return Quaternion(*this).normalise();
//   }
//   
//   void get_euler_angles(float& yaw, float& pitch, float& roll) const {
//     yaw   = atan2(2 * (w * v.z + v.y * v.z), 1 - 2 * (v.y * v.y + v.z * v.z));
//     pitch =  asin(2 * (w * v.y - v.z * v.x));
//     roll  = atan2(2 * (w * v.x + v.y * v.z), 1 - 2 * (v.x * v.x + v.y * v.y));
//   }
// };
// 
// #endif
