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
