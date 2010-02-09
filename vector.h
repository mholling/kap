#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "column.h"
#include <math.h>

class Vector : public Column<3> {
public:
  Vector() { }
  Vector(const Column<3>& column) : Column<3>(column) { }
  Vector(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }
  
  using Column<3>::operator =;  
  
  const Vector cross(const Vector& rhs) const {
    return Vector(data[1] * rhs(2) - data[2] * rhs(1), data[2] * rhs(0) - data[0] * rhs(2), data[0] * rhs(1) - data[1] * rhs(0));
  }
};

#endif




// #ifndef __VECTOR_H_
// #define __VECTOR_H_
// 
// #include "matrix.h"
// #include <math.h>
// 
// class Vector : public Matrix<3, 1> {
// public:
//   inline Vector(const Matrix<3, 1>& matrix) : Matrix<3, 1>(matrix) { }
//   inline Vector(float v = 0.0) : Matrix<3, 1>(v) { }
//   inline Vector(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }
//   
//   inline float& operator()(int m) { return data[m]; }
//   inline const float& operator()(int m) const { return data[m]; }
//   
//   Vector cross(const Vector& rhs) const {
//     return Vector((*this)(1) * rhs(2) - (*this)(2) * rhs(1), (*this)(2) * rhs(0) - (*this)(0) * rhs(2), (*this)(0) * rhs(1) - (*this)(1) * rhs(0));
//   }
//   
//   float dot(const Vector& rhs) const {
//     return (*this)(0) * rhs(0) + (*this)(1) * rhs(1) + (*this)(2) * rhs(2);
//     // return (t() * rhs)(0,0); // TODO: maybe a conversion from Matrix<1, 1> to float?
//   }
//   
//   float sqabs() const { return dot(*this); }
//   float abs() const { return sqrt(sqabs()); }
// 
//   Vector& normalise() {
//     *this /= abs();
//     return *this;
//   }
//   
//   const Vector normalised() const {
//     return Vector(*this).normalise();
//   }
// };
// 
// #endif
