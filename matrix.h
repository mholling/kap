#ifndef __MATRIX_H_
#define __MATRIX_H_

template <int M, int N>
class Matrix {
public:
  float data[M * N];
  inline float& operator()(int m, int n) { return data[m * N + n]; }
  inline const float& operator()(int m, int n) const { return data[m * N + n]; }
  
  Matrix<M, N>& operator +=(const Matrix<M, N>& rhs) { for (int i = 0; i < M * N; i++) data[i] += rhs.data[i]; return *this; }
  const Matrix<M, N> operator +(const Matrix<M, N>& rhs) const { return Matrix<M, N>(*this) += rhs; }

  Matrix<M, N>& operator -=(const Matrix<M, N>& rhs) { for (int i = 0; i < M * N; i++) data[i] -= rhs.data[i]; return *this; }
  const Matrix<M, N> operator -(const Matrix<M, N>& rhs) const { return Matrix<M, N>(*this) -= rhs; }
  
  Matrix<M, N>& operator *=(float k) { for (int i = 0; i < M * N; i++) data[i] *= k; return *this; }
  const Matrix<M, N> operator *(float k) const { return Matrix<M, N>(*this) *= k; }
  
  Matrix<M, N>& operator /=(float k) { for (int i = 0; i < M * N; i++) data[i] /= k; return *this; }
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

  Matrix<M, N>& operator *=(const Matrix<N, N>& rhs) { return (*this) = (*this) * rhs; }
  
  Matrix<M, N>& invert();
  const Matrix<M, N> inverse() const;
  
  Matrix<M, N>& operator /=(const Matrix<N, N>& rhs) { return (*this) *= rhs.inverse(); }
  const Matrix<M, N> operator /(const Matrix<N, N>& rhs) const { return Matrix<M, 2>(*this) /= rhs; }
  
  const Matrix<N, M> transpose() {
    Matrix<N, M> result;
    for (int m = 0; m < M; m++)
      for (int n = 0; n < N; n++)
        result(n, m) = (*this)(m, n);
    return result;
  }
};

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

template <>
const Matrix<2, 2> Matrix<2, 2>::inverse() const { return Matrix<2, 2>(*this).invert(); }

#endif
