#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#include "column.h"
#include "vector.h"
#include <math.h>

class Quaternion : public Column<4> {
public:
  Quaternion() { }
  Quaternion(const Vector& v, float w) { data[0] = v(0); data[1] = v(1); data[2] = v(2); data[3] = w; }
  
  void get_euler_angles(float& yaw, float& pitch, float& roll) const {
    yaw   = atan2(2 * (data[3] * data[2] + data[1] * data[2]), 1 - 2 * (data[1] * data[1] + data[2] * data[2]));
    pitch =  asin(2 * (data[3] * data[1] - data[2] * data[0]));
    roll  = atan2(2 * (data[3] * data[0] + data[1] * data[2]), 1 - 2 * (data[0] * data[0] + data[1] * data[1]));
  }

  const Quaternion cross(const Quaternion& rhs) const {
    Quaternion result;
    for (int n = 0; n < 3; n++) {
      result(n) = data[3] * rhs(n) + rhs(3) * data[n];
      result(3) -= data[n] * rhs(n);
    }
    result(0) -= data[1] * rhs(2) - data[2] * rhs(1);
    result(1) -= data[2] * rhs(0) - data[0] * rhs(2);
    result(2) -= data[0] * rhs(1) - data[1] * rhs(0);
    result(3) += data[3] * rhs(3);
    return result;
  }
  
};

#endif


// #ifndef __QUATERNION_H_
// #define __QUATERNION_H_
//  
// #include "vector.h"
// #include <math.h>
//  
// class Quaternion {
// private:
//   Vector v;
//   float w;
//   
// public:
//   Quaternion() { }
//   Quaternion(const Vector&v, float w) : v(v), w(w) { }
//   Quaternion(float x, float y, float z, float w) : v(x,y,z), w(w) { }
//   
//   void get_euler_angles(float& yaw, float& pitch, float& roll) const {
//     yaw   = atan2(2 * (w * v(2) + v(1) * v(2)), 1 - 2 * (v(1) * v(1) + v(2) * v(2)));
//     pitch =  asin(2 * (w * v(1) - v(2) * v(0)));
//     roll  = atan2(2 * (w * v(0) + v(1) * v(2)), 1 - 2 * (v(0) * v(0) + v(1) * v(1)));
//   }
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
//     Vector cross_product = v.cross(rhs.v);
//     Vector vector = rhs.v * w;
//     w *= rhs.w;
//     w -= v.dot(rhs.v);
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
// };
//  
// #endif
