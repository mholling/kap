#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <math.h>

template <int M, int N>
class Matrix {
protected:
  float data[M * N];
  
public:
  Matrix() { for (int i = 0; i < M * N; i++) data[i] = 0.0; }
  Matrix(const Matrix& matrix) { operator =(matrix); }
  
  const Matrix& operator =(const Matrix& rhs) { for (int i = 0; i < M * N; i++) data[i] = rhs[i]; return *this; }
  
  float& operator ()(int m, int n) { return data[m * N + n]; }
  const float& operator ()(int m, int n) const { return data[m * N + n]; }

  float& operator [](int m) { return data[m]; }
  const float& operator [](int m) const { return data[m]; }

  const Matrix& operator +=(const Matrix& rhs) { for (int i = 0; i < M * N; i++) data[i] += rhs[i]; return *this; }
  const Matrix operator +(const Matrix& rhs) const { return Matrix(*this) += rhs; }

  const Matrix& operator -=(const Matrix& rhs) { for (int i = 0; i < M * N; i++) data[i] -= rhs[i]; return *this; }
  const Matrix operator -(const Matrix& rhs) const { return Matrix(*this) -= rhs; }
  
  const Matrix& operator *=(float k) { for (int i = 0; i < M * N; i++) data[i] *= k; return *this; }
  const Matrix operator *(float k) const { return Matrix(*this) *= k; }
  
  const Matrix& operator /=(float k) { for (int i = 0; i < M * N; i++) data[i] /= k; return *this; }
  const Matrix operator /(float k) const { return Matrix(*this) /= k; }

  template <int P>
  const Matrix<M, P> operator *(const Matrix<N, P>& rhs) const {
    Matrix<M, P> result;
    for (int m = 0; m < M; m++)
      for (int p = 0; p < P; p++)
        for (int n = 0; n < N; n++)
          result(m, p) += (*this)(m, n) * rhs(n, p);
    return result;
  }

  const Matrix& operator *=(const Matrix<N, N>& rhs) { return (*this) = (*this) * rhs; }
  
  const Matrix<N, M>& invert();
  const Matrix<N, M> inverse() const { return Matrix<N, M>(*this).invert(); }

  const Matrix& operator /=(const Matrix<N, N>& rhs) { return (*this) *= rhs.inverse(); }
  const Matrix operator /(const Matrix<N, N>& rhs) const { return Matrix(*this) /= rhs; }
  
  const Matrix<N, M> t() const {
    Matrix<N, M> result;
    for (int m = 0; m < M; m++)
      for (int n = 0; n < N; n++)
        result(n, m) = (*this)(m, n);
    return result;
  }
  
  // Columns:
  Matrix cross(const Matrix& rhs) const;
  float dot(const Matrix& rhs) const { return (t() * rhs)[0]; } 
  float sqabs() const { return dot(*this); }
  float abs() const { return sqrt(sqabs()); }
  Matrix& normalise() { *this /= abs(); return *this; }
  const Matrix normalised() const { return Matrix(*this).normalise(); }

  // Vectors:
  Matrix(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }
  template <int axis> void rotate_90(int times);
  
  // Quaternions:
  Matrix(const Matrix<3, 1>& v, float w) { data[0] = v[0]; data[1] = v[1]; data[2] = v[2]; data[3] = w; }  
  float yaw() const;
  float pitch() const;
  float roll() const;
  
  // 3 x 3 Matrix
  static const Matrix rotation(int axis, float angle);
};

typedef Matrix<3, 1> Vector;
typedef Matrix<4, 1> Quaternion;

// Vector cross-product:
template <>
inline Vector Vector::cross(const Vector& rhs) const {
  return Vector(data[1] * rhs[2] - data[2] * rhs[1], data[2] * rhs[0] - data[0] * rhs[2], data[0] * rhs[1] - data[1] * rhs[0]);
}

// Vector 90-degree rotation:
template <>
template <int axis>
inline void Vector::rotate_90(int times) {
  float& x = data[(axis + 1) % 3];
  float& y = data[(axis + 2) % 3];
  float t = x;
  switch (times) {
    case 1:
      x = -y;
      y = t;
      break;
    case 2:
      x = -x;
      y = -y;
      break;
    case 3:
      x = y;
      y = -t;
      break;
  }
}

// Quaternion cross-product:
template <>
inline Quaternion Quaternion::cross(const Quaternion& rhs) const {
  Quaternion result;
  for (int n = 0; n < 3; n++) {
    result[n] = data[3] * rhs[n] + rhs[3] * data[n];
    result[3] -= data[n] * rhs[n];
  }
  result[0] -= data[1] * rhs[2] - data[2] * rhs[1];
  result[1] -= data[2] * rhs[0] - data[0] * rhs[2];
  result[2] -= data[0] * rhs[1] - data[1] * rhs[0];
  result[3] += data[3] * rhs[3];
  return result;
}

// Quaternion angles:
template <>
inline float Quaternion::yaw() const {
  return atan2(2 * (data[3] * data[2] + data[1] * data[0]), 1 - 2 * (data[1] * data[1] + data[2] * data[2]));
}

template <>
inline float Quaternion::pitch() const {
  return asin(2 * (data[3] * data[1] - data[2] * data[0]));
}

template <>
inline float Quaternion::roll() const {
  return atan2(2 * (data[3] * data[0] + data[1] * data[2]), 1 - 2 * (data[0] * data[0] + data[1] * data[1]));
}

// 2 x 2 matrix inverse:
template <>
inline const Matrix<2, 2>& Matrix<2, 2>::invert() {
  const float determinant = data[0] * data[3] - data[1] * data[2];
  const float temp = data[0];
  data[1] = -data[1];
  data[2] = -data[2];
  data[0] = data[3];
  data[3] = temp;
  return (*this) /= determinant;
}

// 1 x 1 matrix inverse:
template <>
inline const Matrix<1, 1>& Matrix<1, 1>::invert() {
  data[0] = 1.0 / data[0];
  return *this;
}

// 3 x 3 rotation matrix:
template <>
inline const Matrix<3, 3> Matrix<3, 3>::rotation(int axis, float angle) {
  Matrix<3, 3> result;
  result(axis,axis) = 1.0;
  result((axis + 1) % 3, (axis + 1) % 3) = result((axis + 2) % 3, (axis + 2) % 3) = cos(angle);
  result((axis + 2) % 3, (axis + 1) % 3) = sin(angle);
  result((axis + 1) % 3, (axis + 2) % 3) = -sin(angle);
  return result;
}

#endif
