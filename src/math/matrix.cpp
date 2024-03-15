// matrix.cpp

#include <iostream>
#include <stdexcept>
#include "matrix.h"
/*
  o ----------------------> y
  | (0,0) (0,1) ...    (0, m-1)
  | (1,0) (1,1) ...    (1, m-1)
  |  ...        ...    ...
  |  ...        ...    ...
  |  (n-1, 0)   ...  (n-1, m-1)
 \|/
  x
*/
Matrix::Matrix(size_t n, size_t m) : n(n), m(m)
{
  data = new float *[n];
  for (size_t i = 0; i < n; i++)
  {
    data[i] = new float[m];
    for (size_t j = 0; j < m; j++)
      data[i][j] = 0.0;
  }
}

Matrix Matrix::operator*(const Matrix &other) const
{
  if (this->m != other.n)
  {
    throw std::invalid_argument("As dimensões das matrizes são incompatíveis para multiplicação.");
  }

  Matrix result(n, other.m);
  for (size_t i = 0; i < result.n; i++)
  {
    for (size_t j = 0; j < result.m; j++)
    {
      for (size_t k = 0; k < this->m; k++)
      {
        result.data[i][j] += data[i][k] * other.data[k][j];
      }
    }
  }
  return result;
};

void Matrix::print()
{
  for (int x = 0; x < this->n; x++)
  {
    for (int y = 0; y < this->m; y++)
      std::cout << this->getValue(x, y);
    std::cout << "\n";
  }
}