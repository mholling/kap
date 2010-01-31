#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#include <math.h>

class Quaternion {
public:
  Vector v;
  float w;
  
  Quaternion(const Vector&v, float w) : v(v), w(w) { }
  Quaternion(float x, float y, float z, float w) : Quaternion(Vector(x,y,z), w) { }
  
  Quaternion& operator *=(float a) {
    v *= a;
    w *= a;
    return *this;
  }
  
  Quaternion& operator *=(const Quaternion& quaternion) {
    Vector cross_product(v, quaternion.v);
    Vector vector(quaternion.vector);
    vector *= w;
    w *= quaternion.w;
    w -= v * quaternion.v;
    v *= quaternion.w;
    v += vector;
    v -= cross_product;
    return *this;
  }
    
  inline float norm_squared() const { return v.norm_squared() + w * w; }
  inline float norm() const { return sqrt(norm_squared()); }
  
  Quaternion& normalise() {
    float _norm = norm();
    v /= _norm;
    w /= _norm;
    return *this;
  }
};

#endif
