#include "matrix.h"
#include <math.h>

template <>
Matrix<2, 2>& Matrix<2, 2>::invert() {
  const float determinant = data[0] * data[3] - data[1] * data[2];
  const float temp = data[0];
  data[1] = -data[1];
  data[2] = -data[2];
  data[0] = data[3];
  data[3] = temp;
  return (*this) /= determinant;
}

// Vector methods:

template <>
float Matrix<3, 1>::sqabs() const { return dot(*this)(0); }

template <>
float Matrix<3, 1>::abs() const { return sqrt(sqabs()); }

template <>
Matrix<3, 1>::Matrix(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }

template <>
const Matrix<3, 1> Matrix<3, 1>::cross(const Matrix<3, 1>& rhs) const {
  return Matrix<3, 1>((*this)(1) * rhs(2) - (*this)(2) * rhs(1), (*this)(2) * rhs(0) - (*this)(0) * rhs(2), (*this)(0) * rhs(1) - (*this)(1) * rhs(0));
}

// 1 x 1 matrix methods:

template <>
Matrix<1, 1>& Matrix<1, 1>::invert() {
  data[0] = 1.0 / data[0];
  return *this;
}

template <>
Matrix<1, 1>::operator double() { return data[0]; }
