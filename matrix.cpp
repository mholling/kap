#include "matrix.h"
#include <math.h>

// // 2 x 2 matrix inverse:
//
// template <>
// Matrix<2, 2>& Matrix<2, 2>::invert() {
//   const float determinant = data[0] * data[3] - data[1] * data[2];
//   const float temp = data[0];
//   data[1] = -data[1];
//   data[2] = -data[2];
//   data[0] = data[3];
//   data[3] = temp;
//   return (*this) /= determinant;
// }
//
//
// // 1 x 1 matrix inverse:
//
// template <>
// Matrix<1, 1>& Matrix<1, 1>::invert() {
//   data[0] = 1.0 / data[0];
//   return *this;
// }


// // 1 x 1 matrix scalar conversion:
// 
// template <>
// Matrix<1, 1>::operator double() const { return data[0]; }


// TODO: how to include template specialisations in header??


template <>
const Matrix<3, 1> Matrix<3, 1>::cross(const Matrix<3, 1>& rhs) const {
  return Matrix<3, 1>(data[1] * rhs(2) - data[2] * rhs(1), data[2] * rhs(0) - data[0] * rhs(2), data[0] * rhs(1) - data[1] * rhs(0));
}

// template <>
// const Matrix<4, 1> Matrix<4, 1>::cross(const Matrix<4, 1>& rhs) const {
//   Quaternion result;
//   for (int n = 0; n < 3; n++) {
//     result(n) = data[3] * rhs(n) + rhs(3) * data[n];
//     result(3) -= data[n] * rhs(n);
//   }
//   result(0) -= data[1] * rhs(2) - data[2] * rhs(1);
//   result(1) -= data[2] * rhs(0) - data[0] * rhs(2);
//   result(2) -= data[0] * rhs(1) - data[1] * rhs(0);
//   result(3) += data[3] * rhs(3);
//   return result;
// }

