#ifndef __MATRIX_H_
#define __MATRIX_H_

template <int M, int N>
class Matrix {
protected:
  float data[M * N];
  
public:
  Matrix(const Matrix& matrix) { for (int i = 0; i < M * N; i++) data[i] = matrix.data[i]; }
  Matrix(float v = 0.0) { for (int i = 0; i < M * N; data[i++] = v) ; }
  
  inline float& operator()(int m, int n) { return data[m * N + n]; }
  inline const float& operator()(int m, int n) const { return data[m * N + n]; }
  
  inline float& operator()(int m) { return data[m]; }
  inline const float& operator()(int m) const { return data[m]; }

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
  
  Matrix<N, M>& invert();
  const Matrix<N, M> inverse() const { return Matrix<N, M>(*this).invert(); }

  Matrix<M, N>& operator /=(const Matrix<N, N>& rhs) { return (*this) *= rhs.inverse(); }
  const Matrix<M, N> operator /(const Matrix<N, N>& rhs) const { return Matrix<M, N>(*this) /= rhs; }
  
  const Matrix<N, M> t() const {
    Matrix<N, M> result;
    for (int m = 0; m < M; m++)
      for (int n = 0; n < N; n++)
        result(n, m) = (*this)(m, n);
    return result;
  }
  
  template <int P>
  Matrix<M, N + P> operator <<(const Matrix<M, P>& rhs) const {
    Matrix<M, N + P> result;
    for (int m = 0; m < N; m++) {
      for (int n = 0; n < N; n++)
        result(m,n) = (*this)(m,n);
      for (int p = 0; p < P; p++)
        result(m,N + p) = rhs(m,p);
    }
    return result;
  }
  
  const Matrix<M, N + 1> operator<<(float rhs) const { return (*this) << Matrix<M, 1>(rhs); }
  
  template <int P>
  Matrix<N, P> dot(const Matrix<M, P>& rhs) const { return t() * rhs; }
  
  // for Vectors only:
  
  float sqabs() const;
  float abs() const;
  Matrix(float x, float y, float z);
  const Matrix<M, N> cross(const Matrix<M, N>& rhs) const;
  
  // for Matrix<1, 1> only:

  operator double();
};

#endif
