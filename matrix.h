#ifndef __MATRIX_H_
#define __MATRIX_H_

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
    
  // TODO: should this be different? maybe in Vector instead?
  const Matrix<M, N + 1> operator <<(float rhs) const;
  
  
  // // Matrix<1, 1> methods:
  // operator double() const;
};

#endif
