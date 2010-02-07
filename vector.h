#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "matrix.h"
#include <math.h>

class Vector : public Matrix<3, 1> {
public:
  inline Vector() : Matrix<3, 1>() { } // TODO: needed why?
  inline Vector(Matrix<3, 1>) { }
  inline Vector(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }
  
  inline float& operator()(int m) { return data[m]; }
  inline const float& operator()(int m) const { return data[m]; }
  
  Vector cross(const Vector& rhs) const {
    return Vector((*this)(1) * rhs(2) - (*this)(2) * rhs(1), (*this)(2) * rhs(0) - (*this)(0) * rhs(2), (*this)(0) * rhs(1) - (*this)(1) * rhs(0));
  }
  
  float dot(const Vector& rhs) const {
    return (*this)(0) * rhs(0) + (*this)(1) * rhs(1) + (*this)(2) * rhs(2);
    // return (t() * rhs)(0,0); // TODO: maybe a conversion from Matrix<1, 1> to float?
  }
  
  float norm_squared() const { return dot(*this); }
  float norm() const { return sqrt(norm_squared()); }

  Vector& normalise() {
    *this /= norm();
    return *this;
  }
  
  const Vector normalised() const {
    return Vector(*this).normalise();
  }
};

#endif
