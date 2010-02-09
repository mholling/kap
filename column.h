#ifndef __COLUMN_H_
#define __COLUMN_H_

#include "matrix.h"
#include <math.h>

template <int M>
class Column : public Matrix<M, 1> {
public:
  Column() { }
  Column(const Matrix<M, 1>& matrix) : Matrix<M, 1>(matrix) { }
  using Matrix<M, 1>::operator =;
    
  inline float& operator()(int m) { return (*this).data[m]; } // TODO: why is (*this) necessary?
  inline const float& operator()(int m) const { return (*this).data[m]; }
  
  float dot(const Column<M>& rhs) const { return ((*this).t() * rhs)(0,0); }
  float sqabs() const { return (*this).dot(*this); };
  float abs() const { return sqrt(sqabs()); }
  
  Column<M>& normalise() { *this /= abs(); return *this; }
  const Column<M> normalised() const { return Column<M>(*this).normalise(); }
};

#endif
