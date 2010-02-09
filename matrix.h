#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <math.h>

template <int M, int N>
class Matrix {
protected:
  float data[M * N];
  
public:
  Matrix() { } // TODO: correct? should we initialize the data elements to zero?
  // Matrix(const Matrix& matrix) { for (int i = 0; i < M * N; i++) data[i] = matrix.data[i]; }
  Matrix(const Matrix<M, N>& matrix) { *this = matrix; }
  
  const Matrix<M, N>& operator =(const Matrix<M, N>& rhs) { for (int i = 0; i < M * N; i++) data[i] = rhs.data[i]; return *this; }
  
  // TODO: user operator [] instead of operator () !!
  inline float& operator()(int m, int n) { return data[m * N + n]; }
  inline const float& operator()(int m, int n) const { return data[m * N + n]; }

  const Matrix<M, N>& operator +=(const Matrix<M, N>& rhs) { for (int i = 0; i < M * N; i++) data[i] += rhs.data[i]; return *this; }
  const Matrix<M, N> operator +(const Matrix<M, N>& rhs) const { return Matrix<M, N>(*this) += rhs; }

  const Matrix<M, N>& operator -=(const Matrix<M, N>& rhs) { for (int i = 0; i < M * N; i++) data[i] -= rhs.data[i]; return *this; }
  const Matrix<M, N> operator -(const Matrix<M, N>& rhs) const { return Matrix<M, N>(*this) -= rhs; }
  
  const Matrix<M, N>& operator *=(float k) { for (int i = 0; i < M * N; i++) data[i] *= k; return *this; }
  const Matrix<M, N> operator *(float k) const { return Matrix<M, N>(*this) *= k; }
  
  const Matrix<M, N>& operator /=(float k) { for (int i = 0; i < M * N; i++) data[i] /= k; return *this; }
  const Matrix<M, N> operator /(float k) const { return Matrix<M, N>(*this) /= k; }

  template <int P>
  const Matrix<M, P> operator *(const Matrix<N, P>& rhs) const {
    Matrix<M, P> result;
    for (int m = 0; m < M; m++)
      for (int p = 0; p < P; p++)
        for (int n = 0; n < N; n++)
          result(m, p) += (*this)(m, n) * rhs(n, p);
    return result;
  }

  const Matrix<M, N>& operator *=(const Matrix<N, N>& rhs) { return (*this) = (*this) * rhs; }
  
  const Matrix<N, M>& invert();
  const Matrix<N, M> inverse() const { return Matrix<N, M>(*this).invert(); }

  const Matrix<M, N>& operator /=(const Matrix<N, N>& rhs) { return (*this) *= rhs.inverse(); }
  const Matrix<M, N> operator /(const Matrix<N, N>& rhs) const { return Matrix<M, N>(*this) /= rhs; }
  
  const Matrix<N, M> t() const {
    Matrix<N, M> result;
    for (int m = 0; m < M; m++)
      for (int n = 0; n < N; n++)
        result(n, m) = (*this)(m, n);
    return result;
  }
  
  // Vectors:
  Matrix(float x, float y, float z) { data[0] = x; data[1] = y; data[2] = z; }
  const Matrix<M, N> cross(const Matrix<M, N>& rhs) const;
  inline float& operator()(int m) { return data[m]; }
  inline const float& operator()(int m) const { return data[m]; }
  float dot(const Matrix<M, N>& rhs) const { return (t() * rhs)(0); } 
  float sqabs() const { return dot(*this); }
  float abs() const { return sqrt(sqabs()); }
  Matrix<M, N>& normalise() { *this /= abs(); return *this; }
  const Matrix<M, N> normalised() const { return Matrix<M, N>(*this).normalise(); }

  // Quaternions:
  Matrix(const Matrix<3, 1>& v, float w) { data[0] = v(0); data[1] = v(1); data[2] = v(2); data[3] = w; }
  
  void get_euler_angles(float& yaw, float& pitch, float& roll) const {
    yaw   = atan2(2 * (data[3] * data[2] + data[1] * data[2]), 1 - 2 * (data[1] * data[1] + data[2] * data[2]));
    pitch =  asin(2 * (data[3] * data[1] - data[2] * data[0]));
    roll  = atan2(2 * (data[3] * data[0] + data[1] * data[2]), 1 - 2 * (data[0] * data[0] + data[1] * data[1]));
  }
  
};

typedef Matrix<3, 1> Vector;
typedef Matrix<4, 1> Quaternion;

#endif
